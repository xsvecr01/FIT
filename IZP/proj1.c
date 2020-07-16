/**
 * Soubor:  proj1.c
 * Datum:   Rijen 2018
 * Autor:   Radek Svec, xsvecr01@stud.fit.vutbr.cz
 * Projekt: Prace s textem
 * Popis:   Program provádí upravu textu, funguji prikazy a, b, i, n, r, d, g, s. Prikazy s:, S: nefunguji.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_COUNT 100
#define LINE_LEN 1001
#define HELP "Program spoustejte pomoci: ./proj1 JMENO_SOUBORU_S_PRIKAZY <textovy vstup\n"
#define ARG char command[LINE_LEN], char edit_text[LINE_LEN]

///----------------funkce pro oddeleni hodnoty od prikazu------------
int onlycontent(char command[LINE_LEN], char ret_val[LINE_LEN])
{
    int i = 0;
    while (command[i] != '\0')
    {
        ret_val[i] = command[i+1];		///uklada prvky z pole command od prvku s indexem 1, dokud neni prvek roven znaku konce pole
        i++;
    }
    return 0;
}
///---------------------funkce insert-------------------
///vytiskne radek pred aktualnim radkem
int insert(char command[LINE_LEN])
{
	char cmd_val[LINE_LEN];
    onlycontent(command, cmd_val);
	fprintf(stdout, "%s", cmd_val);
	return 0;
}
///---------------------funkce before------------------
///pridani textu za aktualni radek
int before(ARG)
{
    char cmd_val[LINE_LEN];
    onlycontent(command, cmd_val);
    cmd_val[strcspn(cmd_val,"\n")] = 0;	///odstraneni znaku pro newline
    strcat(cmd_val, edit_text);			///spojeni dvou retezcu
	///for pro prohozeni obsahu retezcu
    for(unsigned int i = 0; i <= strlen(cmd_val); i++)
    {
        edit_text[i] = cmd_val[i];
    }
    return 0;

}
///---------------------funkce after-----------------
int after(ARG)
{
    char cmd_val[LINE_LEN];
    onlycontent(command, cmd_val);
    edit_text[strcspn(edit_text,"\n")] = 0;		///odstrani znak newline
    strcat(edit_text, cmd_val);					///pripoji retezec cmd_val za retezec edit_text
    return 0;
}
///---------------------funkce remove EOL-------------
int removeEOL(char edit_text[LINE_LEN])
{
    edit_text[strcspn(edit_text,"\n")] = 0;		///najde znak newline a nahradi ho nulou
    return 0;
}
///--------------------funkce next--------------------
int next(ARG)
{
    char cmd_val[LINE_LEN];
    int count = 0;
    int i = 0;
    onlycontent(command, cmd_val);
    sscanf(cmd_val, "%d", &count);		///ziskani poctu radku k nacteni
    ///cyklus do while pro provedeni funkce minimalne jednou
    do
	{
        fprintf(stdout, "%s", edit_text);	///vytiskne aktualni radek a pak nacte dalsi
        fgets(edit_text, LINE_LEN, stdin);
        i++;
    } while(i <= count);

    return 0;
}
///--------------------------funkce delete---------------
int del(ARG)
{
    char cmd_val[LINE_LEN];
    int count = 0;
    int i = 0;
    onlycontent(command, cmd_val);
    sscanf(cmd_val, "%d", &count);		///ziskani poctu opakovani
    do
    {
        fgets(edit_text, LINE_LEN, stdin);	///pouze nacte dalsi radek
        i++;
    } while(i <= count);

    return 0;
}
///-------------------------funkce goto-------------------
int go(char command[], int j)
{
    char cmd_val[LINE_LEN];
    j = 0;
    onlycontent(command, cmd_val);
    sscanf(cmd_val, "%d", &j);			///ziskani int z pole
    j-=2;                           ///-2 protoze pole se indexuje od 0 a uzivatel zada hodnotu X > 1 a po dokonceni prikazu se provede i++
    return j;
}
///--------------------------funkce getPatRep-------------------
///rozdeluje pattern a replacement
int getPatRep(char cmd_val[LINE_LEN], char pattern[LINE_LEN], char replacement[LINE_LEN])
{
    int j = 0;
    int i = 1;
	///naplnuje pole pattern dokud neni mezera
    while(cmd_val[i] != ' ')
    {
        pattern[j] = cmd_val[i];
        i++;
        j++;
    }
    j = 0;
    i++;
	///naplnuje pole replacement dokud neni znak konce radku nebo konce pole
    while(cmd_val[i] != '\0' && cmd_val[i] != '\n')
    {
        replacement[j] = cmd_val[i];
        i++;
        j++;
    }
    replacement[j] = '\0';
    return 0;
}
///--------------------------funkce s---------------------------
int substitute(ARG)
{
    char cmd_val[LINE_LEN];
    onlycontent(command, cmd_val);
    char pattern[LINE_LEN];
    char replacement[LINE_LEN];
    char new_text[LINE_LEN];
    getPatRep(cmd_val, pattern, replacement);
    char *pat_val;				///ukazatel na pozici patternu
    int i = 0;
    pat_val = strstr(edit_text, pattern);
    while(i < (pat_val - edit_text))	///naplneni zacatku pole hodnotami dokud neni pattern
    {
        new_text[i] = edit_text[i];
        i++;
    }

    int j = 0;
    int k = i;             ///ulozeni hodnoty i do k, pro ulozeni pozice pred pattern
    while(replacement[j] != '\0')		///naplneni pole replacementem
    {
        new_text[i] = replacement[j];
        i++;
        j++;
    }

    while(edit_text[k] != '\0')	///doplneni zbytku pole prvky z pocatecniho pole
    {
        new_text[i] = edit_text[k + strlen(pattern)];
        i++;
        k++;
    }

    new_text[i] = '\0';
	for (unsigned int k = 0; k <= strlen(new_text); k++)	///nacteni noveho pole do puvodniho pole
	{
		edit_text[k] = new_text[k];
	}
    return 0;
}

///-------------------------funkce select----------------------------------
///slouzi pro urceni prikazu
int select (char commands[CMD_COUNT][LINE_LEN])
{
    char str[LINE_LEN]; ///pole pro vstup
    fgets(str, LINE_LEN, stdin); ///nacteni prvniho radku upravovaneho textu

    int i = 0;
    while(commands[i][0])
    {
        switch (commands[i][0])///porovnani prikazu a vyber funkce
        {
            case 'i':
                insert(commands[i]);
                break;
            case 'b':
                before(commands[i], str);
                break;
            case 'a':
                after(commands[i], str);
                break;
            case 'd':
                del(commands[i], str);
                break;
            case 'r':
                removeEOL(str);
                break;
            case 's':
                substitute(commands[i], str);
                break;
            case 'S':
                break;
            case 'n':
                next(commands[i], str);
                break;
            case 'q':
                return 0;
                break;
            case 'g':
                i = go(commands[i], i);
                break;
            default:
                fprintf(stderr, "Neznamy prikaz\n");
                break;
        }
    i++;
    }
	///cyklus pro tisk zbylych radku, pokud uz nejsou prikazy
	do
	{
		fprintf(stdout, "%s", str);
	}
    while(fgets(str, LINE_LEN, stdin) != NULL);

	return 0;
}

///-------------------------funkce main----------------------------------
int main(int argc, char *argv[])
{
    char commands[CMD_COUNT][LINE_LEN];
    int i = 0;

    ///Vypis napovedy pokud nebyl zadan zadny argument.
    if(argc == 1)
    {
        fprintf(stdout, HELP);
        return 0;
    }

    ///Otevreni souboru s prikazy, jeho kontrola a nasledne zavreni souboru
    else if(argc == 2)
    {
        FILE *cf;
        if ((cf = fopen(argv[1],"r")) != NULL)
        {
            ///naplneni pole commands prikazy ze souboru
            while(fgets(commands[i], LINE_LEN, cf))
            {
                i++;
            }
            fclose(cf);
            select(commands);
            return 0;
        }

        else
        {
            fprintf(stderr,"Error: Soubor se nepodarilo otevrit\n");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "Error: Spatne vlozene argumenty\n");
        return -1;
    }
}
