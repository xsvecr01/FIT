# ISA 2020 - Projekt - Varianta Filtrující DNS resolver
**Autor:** Radek Švec (xsvecr01@stud.fit.vutbr.cz)
**Login:** xsvecr01
**Platforma:** Linux

### Specifikace
Program `dns` slouží k filtrování doménových jmen a případnému odepření přístupu uživatele na zakázané domény.
**Jazyk:** C++

### Spouštění programu
**Překlad**
**Program využívá knihovnu lpthread**
Překlad programu je uskutečněn pomocí přiloženého makefile příkazem `make`.
**Spouštění programu**
`./dns -s server [-p port] -f filter_file`, kde:
* `-s server` IP adresa nebo doménové jméno DNS serveru (resolveru), kam se má zaslat dotaz.
* `-p port` Číslo portu, na kterém bude program očekávat dotazy. Výchozí je port 53.
* `-f filter_name` Jméno souboru obsahující nežádoucí domény.

**Příklady spouštění**
```
./dns -s dns.google -p 1234 -f blacklist.txt
./dns -s 8.8.8.8 -p 1234 -f blacklist.txt
```
**Nedostatky programu**
V programu není implementována podpora pro IPV6.
Při velké zátěži na nevýkonném stroji může program skončit neočekávaným způsobem.
Program byl testován pomocí příkazu dig, nejsou k němu žádné unit testy.

**Odevzdané soubory**
```
Makefile
main.cpp
FileParser.hpp
FileParser.cpp
SocketClient.hpp
SocketClient.cpp
SocketServer.hpp
SocketServer.cpp
README.md
manual.pdf
```