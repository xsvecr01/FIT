# ISA 2020: Odpovědní arch pro cvičení č. 1

## Zjišťování konfigurace

### (1.) Rozhraní enp0s3

*MAC adresa*: 08:00:27:e8:97:b6

*IPv4 adresa*: 10.0.2.15

*Délka prefixu*: 64

*Adresa síťe*: 10.0.2.0

*Broadcastová adresa*: 10.0.2.255

### (2.) Výchozí brána

*MAC adresa*: 52:54:00:12:35:02

*IPv4 adresa*: 10.0.2.2

### (4.) DNS servery

*Soubor*: /etc/resolv.conf

*DNS servery*: 192.168.0.1

### (5.) Ping na výchozí bránu

*Soubor*: /etc/hosts

*Úprava*: přidaný řádek: 10.0.2.2    gw

### (6.) TCP spojení

*Záznam + popis*: 
Stav        Recv-Q Send-Q           Lokalni adresa:Port                 Peer adresa:Port
ESTAB       0      0                10.0.2.15:39404                     91.198.174.192:https

Stav: stav socketu
Recv-Q: počet přijatých packetů
Send-Q: počet odeslaných packetů

### (8.) NetworkManager události

*Příkaz*: sudo journalctl -u NetworkManager

### (9.) Chybová hláška sudo

*Příkaz*: sudo journalctl -p 3 -xb | grep wireshark

*Chybová hláška*:
Sep 30 17:36:08 localhost.localdomain sudo[20271]:     user : command not allowed ; TTY=pts/0 ; PWD=/home/user ; USER=root ; COMMAND=/bin/wireshark

## Wireshark

### (1.) Capture filter

*Capture filter*: port http

### (2.) Zachycená HTTP komunikace

Komu patří nalezené IPv4 adresy a MAC adresy?
Mému zařízení, webovému serveru, gateway.

Vypisovali jste již některé z nich?

Proč tomu tak je?

Ano, v prvním úkolu, protože požadavek je odeslán z mého zařízení

#### Požadavek HTTP

Cílová MAC adresa

  - *Adresa*: 52:54:00:12:35:02
  - *Role zařízení*: Výchozí brána

Cílová IP adresa

  - *Adresa*: 147.229.177.179
  - *Role zařízení*: Server

Zdrojová MAC adresa

  - *Adresa*: 08:00:27:e8:97:b6
  - *Role zařízení*: Klient

Zdrojová IP adresa

  - *Adresa*: 10.0.2.15
  - *Role zařízení*: Klient


#### Odpověď HTTP

Cílová MAC adresa

  - *Adresa*: 08:00:27:e8:97:b6
  - *Role zařízení*: Klient

Cílová IP adresa

  - *Adresa*: 10.0.2.15
  - *Role zařízení*: Klient

Zdrojová MAC adresa

  - *Adresa*: 52:54:00:12:35:02
  - *Role zařízení*: Výchozí brána

Zdrojová IP adresa

  - *Adresa*: 147.229.177.179
  - *Role zařízení*: Server

### (3.) Zachycená ARP komunikace

*Display filter*: icmp or arp

### (6.) Follow TCP stream

Jaký je formát zobrazených dat funkcí *Follow TCP stream*, slovně popište
význam funkce *Follow TCP stream*:

Díky funkci můžeme sledovat komunikaci mezi dvěmi zařízeními. Pakety jsou vypisovány chronologicky, data se vypisují ve formě textu. Zařízení jsou barevně odlišeny.
