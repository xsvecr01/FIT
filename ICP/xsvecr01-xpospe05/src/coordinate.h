/**
 * @file coordinate.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida souradnice.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief trida pro souradnici
 * 
 */
class Coordinate
{
private:
    int x;
    int y;
public:

    /**
     * @brief Defaultni konstruktor tridy Coordinate
     * 
     */
    Coordinate();

    /**
     * @brief Konstruktor tridy Coordinate
     * 
     * @param cx souradnice na ose X
     * @param cy souradnice na ose Y
     */
    Coordinate(int cx, int cy);

    /**
     * @brief Navraci pozici na ose X
     * 
     * @return int souradnice osy X
     */
    int getX();

    /**
     * @brief Navraci pozici na ose Y
     * 
     * @return int souradnice osy Y
     */
    int getY();

    /**
     * @brief Vraci rozdil X-ove osy mezi souradnicemi
     * 
     * @param c Coordinate porovnavana souradnice
     * @return int rozdil na ose X (this.x - c.x)
     */
    int diffX(Coordinate c);

    /**
     * @brief Vraci rozdil Y-ove osy mezi souradnicemi
     * 
     * @param c Coordinate porovnavana souradnice
     * @return int rozdil na ose Y (this.y - c.y)
     */
    int diffY(Coordinate c);

    /**
     * @brief Vypocitava vzdalenost mezi dvema souradnicemi
     * 
     * @param c Coordinate porovnavana souradnice
     * @return float vypocitana vzdalenost
     */
    float distance(Coordinate c);

    /**
     * @brief Vypocitava uhel mezi dvema souradnicemi
     * 
     * @param c Coordinate porovnavana souradnice
     * @return float vypocitany uhel
     */
    float angle(Coordinate c);

    /**
     * @brief Porovnava, jestli jsou souradnice shodne
     * 
     * @param c Coordinate porovnavana souradnice
     * @return true pokud se souradnice rovnaji
     * @return false pokud jsou souradnice odlisne
     */
    bool equals(Coordinate c);
};


#endif // COORDINATE_H
