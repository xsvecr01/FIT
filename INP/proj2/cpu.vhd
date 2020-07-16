-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Radek Svec (xsvecr01)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

 type fsm_state is (
	sidle,
	sfetch,
	sdecode, 
	spointer_inc, spointer_dec,
	scell_inc, scell_inc_do,
	scell_dec, scell_dec_do,
	sgetchar, sgetchar_do, 
	sputchar, sputchar_do, 
	ss_while1, ss_while2, ss_while3, ss_while4, ss_while5,
	se_while1, se_while2, se_while3, se_while4, se_while5,
	sact_to_tmp, stmp_to_act, sact_to_tmp_do, stmp_to_act_do,
	shalt, snop
	);
	signal pstate : fsm_state;
	signal nstate : fsm_state;
	
	---------------------------------
	--			Instrukcie
	---------------------------------
	--pre popis viz dekoder
	type instr_type is (
	pointer_inc, pointer_dec,
	cell_inc, cell_dec,
	while_start, while_end,
	putchar, getchar,
	tmp_to_act, act_to_tmp,
	halt, nop
	);
	signal instr : instr_type;

	signal r_pc_addr: std_logic_vector(12 downto 0);
	signal r_pc_inc : std_logic;
	signal r_pc_dec : std_logic;

	signal r_ptr_addr: std_logic_vector(12 downto 0);
	signal r_ptr_inc : std_logic;
	signal r_ptr_dec : std_logic;

	signal r_cnt_addr : std_logic_vector(7 downto 0);
	signal r_cnt_inc : std_logic;
	signal r_cnt_dec : std_logic;

	signal r_mux2_addr : std_logic_vector(12 downto 0);

	signal select1 : std_logic;
	signal select2 : std_logic;
	signal select3 : std_logic_vector(1 downto 0);

begin

--
--mux1
--
	mux1: process(CLK, RESET, select1, r_pc_addr, r_mux2_addr)
	begin
		case select1 is
			when '0' => DATA_ADDR <= r_pc_addr;
			when '1' => DATA_ADDR <= r_mux2_addr;
			when others => null;
		end case;
	end process;

--
--mux2
--
	mux2: process(CLK, RESET, select2, r_ptr_addr)
	begin
		case select2 is
			when '0' => r_mux2_addr <= r_ptr_addr;
			when '1' => r_mux2_addr <= "1000000000000";
			when others => null;
		end case;
	end process;

--
--mux3
--
	mux3: process(CLK, RESET, select3, IN_DATA, DATA_RDATA)
	begin
		case select3 is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= DATA_RDATA - 1;
			when "10" => DATA_WDATA <= DATA_RDATA + 1;
			when "11" => DATA_WDATA <= DATA_RDATA;
			when others => null;
		end case;

	end process;

--
--cnt
--
	cnt_process: process(RESET, CLK)
	begin
		if (RESET = '1') then
			r_cnt_addr <= (others => '0');
		elsif (CLK'event) and (CLK = '1') then
			if (r_cnt_inc = '1') then
				r_cnt_addr <= r_cnt_addr + 1;
			elsif (r_cnt_dec = '1') then
				r_cnt_addr <= r_cnt_addr - 1;
			else null;
			end if;
		end if;
	end process;

--
--pc
--
	pc_process: process(RESET, CLK)
	begin
		if (RESET = '1') then
			r_pc_addr <= (others => '0');
		elsif (CLK'event) and (CLK='1') then
			if (r_pc_inc = '1') then
				r_pc_addr <= r_pc_addr + 1;
			elsif (r_pc_dec = '1') then
				r_pc_addr <= r_pc_addr - 1;
			else null;
			end if;
		end if;
	end process;

--
--ptr
--
	ptr_process: process(RESET, CLK)
	begin
		if (RESET = '1') then
			r_ptr_addr <= "1000000000000";
		elsif (CLK'event) and (CLK = '1') then
			if (r_ptr_inc = '1') then
				if (r_ptr_addr >= "1111111111111") then
					r_ptr_addr <= "1000000000000";
				else
					r_ptr_addr <= r_ptr_addr + 1;
				end if;
			elsif (r_ptr_dec = '1') then
				if (r_ptr_addr <= "1000000000000") then
					r_ptr_addr <= "1111111111111";
				else
					r_ptr_addr <= r_ptr_addr - 1;
				end if;
			else null;
			end if;
		else null;
		end if;
	end process;

--
--decode
--
	decode: process(DATA_RDATA)
	begin
		case (DATA_RDATA) is
			when X"3E" => instr <= pointer_inc; -->
			when X"3C" => instr <= pointer_dec; --<
			when X"2B" => instr <= cell_inc; --+
			when X"2D" => instr <= cell_dec; ---
			when X"5B" => instr <= while_start; --[
			when X"5D" => instr <= while_end; --]
			when X"2E" => instr <= putchar; --.
			when X"2C" => instr <= getchar; --,
			when X"24" => instr <= act_to_tmp; --$ uloz aktualni do tmp
			when X"21" => instr <= tmp_to_act; --! uloz tmp do aktualni
			when X"00" => instr <= halt; --null
			when others=> instr <= nop; --other
		end case;
	end process;

--
--fsm presentstate
--
	fsm_pstate_proces: process(RESET, CLK)
	begin
		if (RESET = '1') then
			pstate <= sidle;
		elsif (CLK'event) and (CLK = '1') then
			if (EN = '1') then
				pstate <= nstate;
			else null;
			end if;
		else null;
		end if;
	end process;

--
--fsm nextstate
--
	fsm_nstate_proces : process(IN_VLD, IN_DATA, DATA_RDATA, OUT_BUSY, pstate, instr, r_cnt_addr, select1, select2, select3)
	begin
--default
		DATA_EN <= '0';
		DATA_RDWR <= '0';
		OUT_DATA <= (others => '0');
		select1 <= '0';
		select2 <= '0';
		select3 <= "11";
		r_pc_inc <= '0';
		r_pc_dec <= '0';
		r_ptr_inc <= '0';
		r_ptr_dec <= '0';
		r_cnt_inc <= '0';
		r_cnt_dec <= '0';
		IN_REQ <= '0';
		OUT_WE <= '0';
		
		case pstate is
--idling
			when sidle =>
				nstate <= sfetch;
--fetch
			when sfetch =>
				nstate <= sdecode;
				DATA_EN <= '1';
				select1 <= '0';
--decode instruction
			when sdecode =>
				case instr is
					when halt =>
						nstate <= shalt;
					when pointer_inc =>
						nstate <= spointer_inc;
					when pointer_dec =>
						nstate <= spointer_dec;
					when cell_inc =>
						nstate <= scell_inc;
					when cell_dec =>
						nstate <= scell_dec;
					when while_start =>
						nstate <= ss_while1;
					when while_end =>
						nstate <= se_while1;
					when getchar =>
						nstate <= sgetchar;
					when putchar =>
						nstate <= sputchar;
					when tmp_to_act =>
						nstate <= stmp_to_act;
					when act_to_tmp =>
						nstate <= sact_to_tmp;
					when others =>
						nstate <= snop;
				end case;
--> right
			when spointer_inc =>
				nstate <= sfetch;
				r_ptr_inc <= '1';
				r_pc_inc <= '1';
-- < left
			when spointer_dec =>
				nstate <= sfetch;
				r_ptr_dec <= '1';
				r_pc_inc <= '1';
-- + plus
			when scell_inc =>
				nstate <= scell_inc_do;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				select1 <= '1';
			when scell_inc_do =>
				nstate <= sfetch;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				select1 <= '1';
				select3 <= "10";
				r_pc_inc	<= '1';
-- - minus
			when scell_dec =>
				nstate <= scell_dec_do;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				select1 <= '1';
			when scell_dec_do =>
				nstate <= sfetch;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				select1 <= '1';
				select3 <= "01";
				r_pc_inc <= '1';
-- . print
			when sputchar =>
				nstate <= sputchar_do;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				select1 <= '1';
			when sputchar_do =>
				nstate <= sputchar_do;
				if (OUT_BUSY = '0') then
					nstate <= sfetch;
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA;
					r_pc_inc <= '1';
				else null;
				end if;
-- , read
			when sgetchar =>
				nstate <= sgetchar_do;
				IN_REQ <= '1';
			when sgetchar_do =>
				nstate <= sgetchar_do;
				IN_REQ <= '1';
				if (IN_VLD = '1') then
					nstate <= sfetch;
					DATA_EN <= '1';
					DATA_RDWR <= '1';
					select1 <= '1';
					select3 <= "00";
					r_pc_inc <= '1';
				else null;
				end if;
-- [ while
			when ss_while1	=>
				nstate <= ss_while2;
				DATA_EN <= '1';
				r_pc_inc <= '1';
				select1 <= '1';
				DATA_RDWR <= '0';
--if(memptr==0)
			when ss_while2 =>
				nstate <= sfetch;
				if (DATA_RDATA = "00000000") then
					nstate <= ss_while3;
					r_cnt_inc <= '1';
				else null;
				end if;
--start while
			when ss_while3	=>
				nstate <= ss_while4;
				DATA_EN <= '1';
				select1 <= '0';
--while(cnt!=0)
			when ss_while4 =>
				nstate <= ss_while5;
				r_pc_inc <= '1';	
				if (instr = while_start) then
					r_cnt_inc <= '1';
				elsif (instr = while_end) then
					r_cnt_dec <= '1';
				else null;
				end if;
--jump to start
			when ss_while5 =>
				nstate <= ss_while3;
				if (r_cnt_addr = "00000000") then
					nstate <= sfetch;
				else null;
				end if;
	
--] while
			when se_while1 =>
				nstate <= se_while2;
				DATA_EN <= '1';
				select1 <= '1';
				DATA_RDWR <= '0';
--start if(memptr==0)
			when se_while2 =>
				nstate <= sfetch;
				if (DATA_RDATA = "00000000") then
					r_pc_inc <= '1';
				else
					nstate <= se_while3;
					r_pc_dec <= '1';
					r_cnt_inc <= '1';
				end if;
	
			when se_while3 =>
				nstate <= se_while4;
				DATA_EN <= '1';
				select1 <= '0';
--while(cnt!=0)
			when se_while4 =>
				nstate <= se_while5;
				if (instr = while_start) then
					r_cnt_dec <= '1';
				elsif (instr = while_end) then
					r_cnt_inc <= '1';
				else null;
				end if;
	
			when se_while5 =>
				nstate <= sfetch;
				if (r_cnt_addr = "00000000") then
					r_pc_inc <= '1';
				else
					nstate <= se_while3;
					r_pc_dec <= '1';
				end if;	
-- $ tmp = *ptr
			when sact_to_tmp =>
				nstate <= sact_to_tmp_do;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				select1 <= '1';
				select2 <= '0';
			when sact_to_tmp_do =>
				nstate <= sfetch;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				select1 <= '1';
				select2 <= '1';
				select3 <= "11";
				r_pc_inc	<= '1';
-- ! *ptr = tmp
			when stmp_to_act =>
				nstate <= stmp_to_act_do;
				select1 <= '1';
				select2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
			when stmp_to_act_do =>
				nstate <= sfetch;
				select1 <= '1';
				select2 <= '0';
				select3 <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				r_pc_inc	<= '1';
			
--halt
			when shalt =>
				nstate <= shalt;
--nop
			when snop =>
				nstate <= sfetch;
				r_pc_inc <= '1';
	
			when others => null;
	
		end case;
	end process;
end behavioral;