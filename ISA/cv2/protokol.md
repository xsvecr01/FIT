# ISA 2020: Odpovědní arch pro cvičení č. 2

## Vzdálený terminál - SSH, Secure Shell

### (2.) Bezpečné připojení na vzdálený počítač bez autentizačních klíčů

*Verze OpenSSH*: 7.4

*Vybraná podporovaná šifra*: vybraná je chacha2-poly1305, podporovaných je mnoho

*Co lze zjistit o obsahu komunikace?*: Komunikace je zašifrována

### (3.) Vytvoření veřejného a privátního klíče

*Jak se liší práva mezi souborem s privátním a veřejným klíčem?*:
id_rsa je privátní
id_rsa.pub je veřejný

### (4.) Distribuce klíčů

*Jaká hesla bylo nutné zadat při kopírovaní klíčů?*:
heslo vzdáleného stroje: pro uživatele - user4lab
                         pro správce - root4lab

*Jaká hesla bylo nutné zadat při opětovném přihlášení?*:
heslo klíče: fitvutisa


Při omezení použití klíčů se po přihlášení jako root spustil zadaný příkaz.

### (6.) Pohodlné opakované použití klíče

*Museli jste znovu zadávat heslo?*: Po opakovaném přihlašování už není nutno heslo zadávat

## Zabezpečení transportní vrstvy - TLS, Transport Layer Security

### (1.) Nezabezpečený přenos dat

*Je možné přečíst obsah komunikace?*: Ano, je možné přečíst kompletní komunikaci - dotaz i odpověď

### (2.) Přenos dat zabezpečený TLS

*Je možné přečíst obsah komunikace?*: není. Ale jde vyčíst jméno serveru
