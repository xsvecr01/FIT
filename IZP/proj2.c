/**
 * Soubor:  proj2.c
 * Datum:   Listopad 2018
 * Autor:   Radek Svec, xsvecr01@stud.fit.vutbr.cz
 * Projekt: Iteracni vypcoty
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
const char *HELP =  "Program spustte pomoci:\n"
                    "'./proj2 --log X N'   pro vypocet logaritmu\n"
                    "'./proj2 --pow X Y N' pro vypocet exponencialni funkce\n";


///-------prototypy funkcí-----------------------
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);


///-------vypocet logaritmu - taylor----------
double taylor_log(double x, unsigned int n)
{
    double t = 0.0, s = 0.0;
    ///---pro hodnoty mensi jak 1
    if(x > 0 && x < 1)
    {
        double x0 = (1-x);
        for(unsigned int i = 1; i<=n; i++)
        {
            t = (x0)/i;
            x0 *= (1-x);
            s = s - t;
        }
    return s;
    }
    ///---pro hodnoty vetsi a rovno 1
    else if(x >= 1)
    {
        double x0 = ((x-1)/x);
        for(unsigned int i = 1; i<=n; i++)
        {
            t = x0/i;
            x0 *= ((x-1)/x);
            s = s + t;
        }
        return s;
    }
    else if (x == 0) return -(INFINITY);
    else return NAN;
}
///---------vypocet logaritmu - zretezene zlomky----------------
double cfrac_log(double x, unsigned int n)
{
    if (x > 0)
    {
        double cf = 1.0;
        double b = 0.0;
        x = (x-1)/(x+1);
        n--;
        for(; n>=1; n--)
        {
            b = 2*n-1;
            cf = b-((n*n*x*x)/cf);
        }
        return (2*x)/cf;
    }
    else if (x == 0) return -(INFINITY);
    else return NAN;
}
///--------vypocet exponencialni funkce - taylor----------------
double taylor_pow(double x, double y, unsigned int n)
{
    if (x == 0 && y < 0) return INFINITY;
    else if (x == 0 && y == 0) return 1;
	else if (x < 0) return -NAN;
    else if (x == 0) return 0;

    double log = taylor_log(x, n);
    double temp;
    double sum = 1;
    double cit1 = 1, cit2 = 1;
    double fact = 1.0;

	for(unsigned int i = 1; i < n ; i++)
        {
            cit1 *= y;
            cit2 *= log;
            fact *= i;
            temp = cit1 * cit2 / fact;
            sum += temp;
        }
    return sum;
}

///--------vypocet exponencialni funkce - cfrac--------------
double taylorcf_pow(double x, double y, unsigned int n)
{
    if (x == 0 && y < 0) return INFINITY;
    else if (x == 0 && y == 0) return 1;
	else if (x < 0) return -NAN;
    else if (x == 0) return 0;

    double log = cfrac_log(x, n);
    double temp;
    double sum = 1;
    double cit1 = 1, cit2 = 1;
    double fact = 1.0;

    for(unsigned int i = 1; i < n ; i++)
        {
            cit1 *= y;
            cit2 *= log;
            fact *= i;
            temp = cit1 * cit2 / fact;
            sum += temp;
        }
    return sum;
}

///-----------------main----------------------
int main(int argc, char *argv[])
{
    ///-----------podminky pro --log-------------
    if(argc == 4 && strcmp(argv[1], "--log") == 0)
    {
        double X = atof(argv[2]);
        unsigned int N = atoi(argv[3]);
        if (N <= 0) fprintf(stderr, "Spatne vlozeny pocet iteraci N musi byt aspon 1\n");
        else
        {
            printf("       log(%g) = %.12g\n", X, log(X));
            printf(" cfrac_log(%g) = %.12g\n", X, cfrac_log(X, N));
            printf("taylor_log(%g) = %.12g\n", X, taylor_log(X, N));
        }
        return 0;
    }

    ///-------------podminky pro --pow--------------
    else if(argc == 5 && strcmp(argv[1], "--pow") == 0)
    {
        double X = atof(argv[2]);
        double Y = atof(argv[3]);
        unsigned int N = atoi(argv[4]);
        if (N <= 0) fprintf(stderr, "Spatne vlozeny pocet iteraci: N musi byt aspon 1\n");
        else
        {
            printf("         pow(%g, %g) = %.12g\n", X, Y, pow(X, Y));
            printf("  taylor_pow(%g, %g) = %.12g\n", X, Y, taylor_pow(X, Y, N));
            printf("taylorcf_log(%g, %g) = %.12g\n", X, Y, taylorcf_pow(X, Y, N));
        }
        return 0;
    }

    else printf(HELP);      ///vypis napovedy
    return 0;
}
