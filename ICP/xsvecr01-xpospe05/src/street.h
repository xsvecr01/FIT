/**
 * @file street.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida ulice.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef STREET_H
#define STREET_H

#include "coordinate.h"
#include "stop.h"
#include <string>
#include <initializer_list>
#include <list>

using namespace std;

class Stop;

/**
 * @brief trida pro ulici
 * 
 */
class Street
{
private:
    string id;
    list<Coordinate> coordinates;
    list<Stop> stops;
public:

    /**
     * @brief Defaultni konstruktor tridy Street
     * 
     */
    Street();

    /**
     * @brief Konstruktor tridy Street
     * 
     * @param i string nazev ulice
     * @param coords list<Coordinate> seznam souradnic, kterymi ulice prochazi
     */
    Street(string i, list<Coordinate> coords);

    /**
     * @brief Vraci jmeno ulice
     * 
     * @return string jmeno ulice
     */
    string getId();

    /**
     * @brief Vraci seznam souradnic, kterymi ulice prochazi
     * 
     * @return list<Coordinate> souradince ulice
     */
    list<Coordinate> getCoordinates();

    /**
     * @brief Vraci seznam zastavek, ktere lezi na ulici
     * 
     * @return list<Stop> seznam zastavek ulice
     */
    list<Stop> getStops();

    /**
     * @brief Vraci pocatek ulice
     * 
     * @return Coordinate pocatecni souradince ulice
     */
    Coordinate begin();

    /**
     * @brief Vraci konec ulice
     * 
     * @return Coordinate posledni souradnice ulice
     */
    Coordinate end();

    /**
     * @brief Pridava zastavku do ulice, pokud zastavka na ulici lezi
     * 
     * @param stop Stop pridavana zastavka
     * @return true pokud se zastavku podarilo pridat
     * @return false pokud zastavka na ulici nelezi a nebyla pridana
     */
    bool addStop(Stop stop);

    /**
     * @brief Kontroluje, zda na sebe ulice navazuji
     * 
     * @param s Street kontrolovana ulice
     * @return true pokud na sebe ulice navazuji
     * @return false pokud na sebe ulice nenavazuji
     */
    bool follows(Street s);
};

#endif // STREET_H
