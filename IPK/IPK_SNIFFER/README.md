# IPK 2020 - Projekt 2. - Varianta ZETA: Sniffer paketů
**Autor:** Radek Švec (xsvecr01@stud.fit.vutbr.cz)
**Login:** xsvecr01
**Platforma:** Linux

### Soubory
ipk-sniffer.c
makefile
README.md
manual.pdf

### Specifikace
Program `ipk-sniffer` je síťový analyzátor v jazyce **C**, který umí na zvoleném síťovém rozhraní zachytávat a filtrovat pakety.
Podporované typy paketů jsou TCP a UDP, a verze IP adresy IPv4.
IPv6 program nepodporuje.

### Spouštění programu
**Překlad**
Překlad programu je uskutečněn pomocí přiloženého makefile příkazem `make`.
**Volání programu**
`./ipk-sniffer -i rozhraní [-p port] [--tcp|-t] [--udp|-u] [-n num]`, kde:
* `-i eth0` (rozhraní, na kterém se má poslouchat, je-li zadáno pouze -i vypíše se seznam aktivních rozhraní)
* `-p 23` (filtrování paketu podle zadaného portu, není-li uvedeno, jsou uvažovány všechny porty)
* `-t` nebo `--tcp` (zobrazení pouze TCP paketů)
* `-u` nebo `--udp` (zobrazení pouze UDP paketů)
Pokud není `-udp` ani `-tcp` zadáno, uvažují se TCP i UDP pakety.
* `-n 10` (určuje počet paketů, které se mají zobrazit, pokud není zadáno, je zobrazen pouze 1 paket)
* Pokud není zadán žádný argument, vypíše se jednoduchá nápověda
* `--help` vypíše celou nápovědu

**Příklady spouštění**
```
./ipk-sniffer -i enp0s3 -p 23 --tcp -n 2
./ipk-sniffer -i enp0s3 --udp
./ipk-sniffer -i enp0s3 -n 10
./ipk-sniffer -i enp0s3 -p 80 --tcp --udp
./ipk-sniffer -i enp0s3 -p 80
./ipk-sniffer -i enp0s3
```
**Pro spuštění programu a správné zachytávání paketů je nutno spouštět s administorskými právy - pomocí příkazu `sudo`.**