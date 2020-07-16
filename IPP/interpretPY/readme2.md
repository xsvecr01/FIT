Implementační dokumentace k 2. úloze do IPP 2019/2020
Jméno a příjmení: Radek Švec
Login: xsvecr01

### interpret.py
Jazyk: `python3`
Použité knihovny: `print_function`, `sys`, `xml.etree.ElementTree`, `re`

## Argumenty a spouštění skriptu
Skript se spouští pomocí:
`python3.8 interpret.py [argumenty]`
Kde argumenty jsou:
`--help` - zobrazí nápovědu skriptu
`--source=file` - vstupní soubor s XML reprezentací zdrojového kódu v jazyce IPPCode20
`--input=file` - vstupní soubor se vstupy pro interpretaci
Alespoň jeden z argumentů `--source=file` a `--input=file` musí být zadán. Pokud jeden chybí, načítají se příslušná data ze standardního vstupu.

## Popis implementace `interpret.py`
Po spuštění skript načítá a zpracovává zadané argumenty pomocí funkce `argparse()`. Poté proběhne syntaktická analýza zdrojového kódu ve funkci `checksyntax()`, která vrací slovník instrukcí. Tento slovník je seřazen podle orderu instrukce a přeindexován. Funkce `check_labels()` projde seřazený slovník instrukcí a vyhledá definici labelů, zároveň kontroluje duplicitní definice labelů. Nakonec se v cyklu volá funkce pro vykonávání instrukcí `run_code()`.

## Globální proměnné
* GF - slovník, globální rámec proměnných
* LF - list, zásobník rámců
* TF - slovník, dočasný rámec proměnných
* STACK - list, datový zásobník
* Labels - slovník labelů
* tmp_pCNT - zásobník volání, ukládá se do něj hodnota programového čítače
* pCNT - programový čítač

## Jednotlivé funkce
**`argparse()`**
Zpracovává zadané argumenty z příkazové řádky a vrací dvojici `(sourcef, inputf)`.
**`checksyntax(root, input)`**
Kontroluje syntaxi vstupního souboru s XML reprezentací kódu. Vrací slovník instrukcí.
**`issymb(m_value, m_type)`, `islabel(m_value, m_type)`, `isvar(m_value, m_type)`, `istype(m_value, m_type)`**
Kontrolují pomocí regulárních výrazů syntaxi pro typ `symbol`, `label`, `var` nebo `type`. Vrací `True` nebo `False`.
**`check_labels(sor_dict)`**
Ve slovníku instrukcí vyhledává definice labelů, hledá duplicitu definice a vyhledané labely spolu s jejich pozicí v kódu ukládá do globálního slovníku `Labels`.
**`get_var(var)`**
Vyhledá a vrátí proměnnou uloženou v GF, LF nebo TF.
**`set_var(var, value, m_type)`**
Vloží proměnnou, její hodnotu a typ do GF, LF nebo TF.
**`escape(var)`**
Překládá escape sekvenci na znak.
**`run_code(line)`**
Zpracovává vždy jednu instrukci uloženou ve slovníku line, a podle operačního kódu `opcode` provádí interpretaci instrukce. Ve většině instrukcí se vždy nejdříve provede kontrola pokud je proměnná pro vykonávanou operaci definována a jestli má správný typ.