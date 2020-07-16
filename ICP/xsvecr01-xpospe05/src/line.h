/**
 * @file line.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida linky
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef LINE_H
#define LINE_H

#include <string>
#include <list>
#include <vector>

#include "street.h"
#include "stop.h"

class Stop;
class Street;

/**
 * @brief trida pro linku
 * 
 */
class Line
{
private:
    string id;
    list<Street> streets;
    list<Stop> stops;
public:  

    /**
     * @brief pole obsahujici minuty jizdniho radu
     * 
     */
    vector<float> stopMinute;
    /**
     * @brief pole obsahujici hodiny jizdniho radu
     * 
     */
    vector<float> stopHour;
    /**
     * @brief Defaultni konstruktor tridy Line
     * 
     */
    Line();

    /**
     * @brief Konstruktor tridy Line
     * 
     * @param i string nazev linky
     */
    Line(string i);

    /**
     * @brief Pridava ulici se zastavkou do linky
     * 
     * @param stre Street pridavana ulice
     * @param sto Stop zastavka pridavane ulice
     * @return true pokud se dvojici podarilo pridat
     * @return false pokud objekty nebyly pridany
     */
    bool addStreet(Street stre, Stop sto);

    /**
     * @brief Pridava samotnou ulici do linky
     * 
     * @param stre Street pridavana ulice
     * @return true pokud ulice byla pridana
     * @return false pokud ulice nebyla pridana
     */
    bool addStreet(Street stre);

    /**
     * @brief Vraci nazev linky
     * 
     * @return string nazev linky
     */
    string getId();
    
    /**
     * @brief Vraci seznam ulic, kterymi linka vede
     * 
     * @return list<Street> seznam ulic linky
     */
    list<Street> getStreets();

    /**
     * @brief Vraci seznam zastavek, kterymi linka vede
     * 
     * @return list<Stop> seznam zastavek, kterymi linka vede
     */
    list<Stop> getStops();

};

#endif // LINE_H
