# ISA 2020: Odpovědní arch pro cvičení č. 3

## (1.) Resolving DNS dotazů

Jaké jsou autoritativní DNS servery pro doménu *vutbr.cz*?\
pipit.cis.vutbr.cz
rhino.cis.vutbr.cz

*Display filter* pro zobrazení pouze DNS provozu:
dns

Počet zachycených paketů souvisejících s vyhledáním NS pro doménu *vutbr.cz*:
2

Provedený DNS dotaz (vyberte správnou variantu): **rekurzivní** — **iterativní**
rekurzivní

Podle čeho jste zjistili zakroužkovaný typ DNS dotazu v zachyceném paketu?
Podle hodnoty u DNS Flags - Recursion desired, Recursion available

Cílová IP adresa paketu s DNS dotazem:
192.168.0.1

Jakému zařízení náleží zapsaná IP adresa?
Výchozí DNS server

## (2.) Zabezpečení a resolving pomocí DNS over HTTPS

Dokážete zjistit ze zachyceného DNS provozu, jaké domény jste předtím navštívili? Proč?   
Ne, protože komunikace je zašifrována

*Display filter* pro zobrazení pouze TLS provozu:
tls

Jeden řádek z položky *Answers* z libovolné DoH odpovědi:  
sdn.cz: type AAAA, class IN, addr 2a02:598:6666:1:14::3


IP adresa, na kterou směřovaly pakety s DoH dotazem:
193.17.47.1

Doménové jméno patřící k doplněné IP adrese:
odvr.nic.cz

## (3.) Zabezpečení a resolving pomocí DNS over TLS

*Display filter* pro zobrazení pouze provozu využívající TCP port 853:
tcp.port == 853

*Display filter* pro zobrazení pouze provozu využívající TCP nebo UDP port 53:
tcp.port == 53 || udp.port == 53

Služba běžící nad portem 53:
DNS

Počet zachycených paketů se zdrojovým nebo cílovým portem 53:
0

## (4.) Blokování reklam a další
Jaký rozdíl jste zpozorovali na webu *idnes.cz* při jeho načtení s aktivním nástrojem *pi-hole*?
Na webu se nezobrazují reklamy