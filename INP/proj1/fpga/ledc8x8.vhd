-- Autor reseni: RADEK ŠVEC, xsvecr01

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
	SMCLK, RESET : in std_logic;
	ROW, LED : out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is

	signal hodiny : std_logic := '0';
	signal hodiny_citac : std_logic_vector(7 downto 0) := "00000000";
	signal radkose : std_logic_vector(7 downto 0) := "10000000";
	signal ledose : std_logic_vector(0 to 7) := "11111111";
	signal interval : std_logic_vector(0 to 22);
	signal sviti_furt : std_logic := '0';

begin

	citacka: process(SMCLK, RESET)
	begin
		if RESET = '1' then
			hodiny_citac <= "00000000";
			interval <= "00000000000000000000000";
		elsif SMCLK'event and SMCLK = '1' then
			hodiny_citac <= hodiny_citac + 1;
			interval <= interval + 1;
		end if;

		if (hodiny_citac = X"FF") then
			hodiny <= '1';
		else 
			hodiny <= '0';
		end if;

		if (interval > "11011111101000100000000") then
			sviti_furt <= '1';
		end if;
		
	end process citacka;
	
	

	radkose_zmena: process(SMCLK, RESET)
	begin
		if RESET = '1' then
			radkose <= "10000000";
		elsif SMCLK'event and SMCLK = '1' and hodiny = '1' then
			radkose <= radkose(0) & radkose(7 downto 1);
		end if;
	end process radkose_zmena;


	blikani: process(SMCLK, RESET)
	begin
		if SMCLK'event and SMCLK = '1' then
			--0.5 / (1/freq) 
			if interval <= "01101111110100010000000" then
				LED <= ledose;
				ROW <= radkose;
			elsif sviti_furt = '1' then
				LED <= ledose;
				ROW <= radkose;
			elsif interval > "01101111110100010000000" and interval < "11011111101000100000000" then
				LED <= "11111111";
				ROW <= radkose;
			else
				LED <= ledose;
				ROW <= radkose;
			end if;
		end if;
	end process blikani;



	dekoder: process(radkose)
	begin
		case radkose is
			when "10000000" => ledose <= "00011111";
			when "01000000" => ledose <= "01101111";
			when "00100000" => ledose <= "00011111";
			when "00010000" => ledose <= "01010000";
			when "00001000" => ledose <= "01100111";
			when "00000100" => ledose <= "11110000";
			when "00000010" => ledose <= "11111110";
			when "00000001" => ledose <= "11110000";
			when others => ledose <= "11111111";
		end case;
	end process dekoder;

end main;