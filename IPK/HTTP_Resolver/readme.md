# IPK - Počítačové komunikace a sítě
Projekt 1. - HTTP resolver doménových jmen
**Autor:** Radek Švec (xsvecr01@stud.fit.vutbr.cz)
**Login:** xsvecr01

### Specifikace
Cílem projektu je implementace severu, který bude komunikovat protokolem HTTP a bude zajišťovat překlad doménových jmen.
Server podporuje operace **GET** a **POST**

### Spouštění
**Pro spuštění je nutno mít nainstalovaný python3**
Pomocí `make run PORT="číslo portu"`
Číslo portu musí být v rozmezí od 1024 do 65535
např.
`make run PORT=1234`
spustí server na portu 1234

### Zvolený programovací jazyk: 
* python3

### Použité knihovny:
* socket
* re
* sys

### Návratové chyby:
* 1 - špatně zadané číslo portu

### Implementace 
Argumenty se kontrolují pomocí knihovny `sys`.
Program využívá pro komunikaci a vytvoření serveru knihovnu `socket`. Pomocí regulárních výrazů (knihovny `re`) se kontrolují typy požadavků, a tvary adres požadovaných pro přeložení.
O hlavní část se stará funkce `resolf()`, která má 2 parametry - seznam vstupních adres a požadovanou metodu (GET nebo POST). Pokud je adresa ve správném formátu, pak se přeloží pomocí příslušné funkce (pro URL:A je to `socket.gethostbyname()` a pro IP:PTR `socket.gethostbyaddr()`) a připojí se do výstupní odpovědi. V případě že adresa nelze přeložit, funkce pro překlad vrací výjimku a HTTP hlavičku 404. Pokud je adresa ve špatném tvaru (např. URL:PTR), pak vrací hlavičku 400.

### Verze HTTP: 1.1

### Příklady správných vstupů pro GET:
```
GET /resolve?name=apple.com&type=A HTTP/1.1
GET /resolve?name=147.229.14.131&type=PTR HTTP/1.1
```

### Příklady správných vstupů pro POST:
```
www.fit.vutbr.cz:A
www.fit.vutbr.cz   :A
www.fit.vutbr.cz  :     A
147.229.14.131:PTR
147.229.14.131     :PTR
147.229.14.131   :    PTR
```
### Návratové hlavičky HTTP/1.1:
#### 200 OK
Pro správný tvar požadavku.
Vrátí seznam nalezených odpovědí, nenalezené odpovědi se ignorují.
např. pro vstup
```
www.fit.vutbr.cz:PTR
www.google.com:A
www.brno.cz:A
34.213.147.57:A
147.229.2.90:PTR
```
je výstup následující:
```
www.google.com:A=216.58.201.100
www.brno.cz:A=185.62.110.180
147.229.2.90:PTR=piranha.ro.vutbr.cz
```

#### 400 Bad Request
Špatný tvar požadavku.
např. `www.fit.vutbr.cz:PTR`
ale i `www.fit.vutbrrrrrrr.cz:PTR` (nebo prázdný řádek u **POST**) vrací *400 Bad Request*.

#### 404 Not Found
V případě, že není nalezena žádná platná odpověď.
např. `www.neeeeeeexistujicidomena.pes:A`

#### 404 nebo 400
Pokud jsou na vstupu u **POST** všechny požadavky chybné (vrací 404 nebo 400), pak je zvolena návratová hlavička podle posledního požadavku.
tzn. že pro vstup
```
www.google.com:AB
147.229.2.903:PTR
```
Je výstup *404 Not Found*
a pro vstup
```
www.google.conm:A
147.229.2.90:A
```
je výstup *400 Bad Request*

#### 405 Method Not Allowed
Pokud je požadovaná metoda jiná, než GET nebo POST.

#### 500 Internal Server Error
Pro ostatní nestandardní případy.