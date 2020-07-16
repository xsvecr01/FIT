Implementační dokumentace k 2. úloze do IPP 2019/2020
Jméno a příjmení: Radek Švec
Login: xsvecr01

## Shrnutí celkové implementace `parse.php`
Skript načte celý vstup do proměnné `input`. Z této proměnné pomocí funkce `preg_replace()` postupně odstraní komentáře, přebytečné mezery a prázdné řádky před hlavičkou. Následně se `input` rozdělí na jednotlivé řádky a ty na jednotlivá slova. Výsledný tvar proměnné `input` je dvourozměrné pole.
Probíhá kontrola hlavičky a případné ukončení s chybou `21`, pokud není nalezena.
V hlavním cyklu se prochází vždy první slovo na řádku, to představuje instrukci. Kontroluje se typ instrukce a podle toho správný počet a tvar argumentů. Pokud je vše OK, zavolá se funkce `m_genxml()`, která se stará o generování XML výstupu. Toto se provede postupně pro všechny řádky.
Nakonec se na standardní výstup vytiskne vygenerované XML.
Pro generování XML formátu se využívá třída `xmlwriter`.

## Jednotlivé funkce
**`m_isconst($const)`**
Pomocí regulárních výrazů kontroluje, zda se jedná o konstantu ve správném tvaru. Návratová hodnota je string obsahující název konstanty (např. `"int"`).
**`m_isvar($var)`**
Vrací `"var"` pokud je proměnná správně zapsána.
**`m_islabel($label)`**
Vrací `"label"` pokud je label ve správném tvaru.
**`m_istype($type)`**
Vrací `"type"` pokud je type ve správném tvaru.
**`m_issymb($symb)`**
Pokud se jedná o symbol, vrací string s typem symbolu (např. `"nil"`).
**`m_genxml($line, $outputXML, $key)`**
Funkce ukládá do globální proměnné `$outputXML` generovaný XML výstup. Pomocí `xmlwriter_start_element()` se vytváří odsazení a elementy pro jednotlivé instrukce. K té se generují atributy *order* a *opcode* pomocí funkce `xmlwriter_write_attribute`. Následně se cyklem generují všechny argumenty a jejich atributy.