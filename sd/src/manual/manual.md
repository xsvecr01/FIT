# Manuál - zařízení s modelářskými servy 
**Autor:** Radek Švec (xsvecr01@stud.fit.vutbr.cz)

## Arduino projekt pro ESP32
Pro nahrání programu na vývojovou desku ESP32 je nutno mít nainstalované prostředí Arduino IDE dostupné z: https://www.arduino.cc/en/software

### Přidání desky ESP32 a projektu do Arduino IDE
Obsah adresáře `src\arduino` si zkopírujte do složky `C:\Users\username\Documents\Arduino`.

Otevřete projekt `hexapod_main.ino` z adresáře `hexapod_main`.

Po spuštění Arduino IDE zvolte desku z nabídky **Nástroje->Vývojová deska->ESP32 Arduino**, v této práci se jedná o desku **DOIT ESP32 DEVKIT V1**.

Nakonec připojte desku k počítači a zvolte odpovídající komunikační port v nabídce **Nástroje->Port**.

Projekt lze ve vývojovém prostředí nahrát pomocí ikonky šipky nebo v nabídce **Projekt->Nahrát**.

## Aplikace pro Android
Pro používání aplikace je vyžadováno zařízení s operačním systémem Android, verze 6.0 a vyšší.

### Instalace
Instalace aplikace mobilní zařízení vyžaduje povolení instalace aplikací z neznámých zdrojů.

Následně lze aplikaci `release/HexControl.apk` zkopírovat do úložiště zařízení a pomocí aplikace pro správu souborů ji otevřít a nainstalovat. 

### Import projektu do vývojového prostředí Android Studio
Android Studio je dostupné z: https://developer.android.com/studio

Projekt lze importovat pomocí nabídky **File->New->Import Project** a zvolením souboru `HexControl.zip` v adresáři `src\android`.

## Spuštění a ovládání aplikace
V prvním kroku je nutné zapnout robota, aby se spustil WiFi přístupový bod na desce ESP32. Pomocí mobilního zařízení se lze na tento přístupový bod připojit.

SSID sítě: `HexapodRobot`

Heslo: `xsvecr01`

Nyní lze robota ovládat prostřednictvím mobilní aplikace.