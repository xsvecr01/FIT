/**
 * @file stop.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida zastavky. 
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef STOP_H
#define STOP_H

#include "coordinate.h"
#include "street.h"
#include <string>

using namespace std;

class Street;

/**
 * @brief trida pro zastavku
 * 
 */
class Stop
{
private:
    string id;
    Street* street;
    Coordinate coordinate;
public:

    /**
     * @brief Defaultni konstruktor pro tridu Stop
     * 
     */
    Stop();

    /**
     * @brief Konstruktor tridy Stop
     * 
     * @param i string jmeno zastavky
     * @param c Coordinate pozice zastavky
     */
    Stop(string i, Coordinate c);

    /**
     * @brief Prirazuje zastavce ulici
     * 
     * @param s Street ulice, na kterou je zastavka pridavana
     */
    void setStreet(Street* s);

    /**
     * @brief Vraci ulici, na ktere zastavka lezi
     * 
     * @return Street* ulice, na ktere je zastavka
     */
    Street* getStreet();

    /**
     * @brief Vraci pozici zastavky
     * 
     * @return Coordinate pozice zastavky
     */
    Coordinate getCoordinate();

    /**
     * @brief Vraci jmeno zastavky
     * 
     * @return string jmeno zastavky
     */
    string getId();
};

#endif // STOP_H
