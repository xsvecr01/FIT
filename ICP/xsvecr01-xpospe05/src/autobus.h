/**
 * @file autobus.h
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida autobusu.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef AUTOBUS_H
#define AUTOBUS_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "line.h"

/**
 * @brief trida pro autobus
 * 
 */
class Autobus : public QGraphicsItem
{
public:

    /**
     * @brief identifikator autobusu
     * 
     */
    string id;
    /**
     * @brief barva autobusu
     * 
     */
    QBrush Brush;
    /**
     * @brief posledni navstivena zastavka
     * 
     */
    Stop lastStop;
    /**
     * @brief pomocna promenna pro zastavku
     * 
     */
    Stop tempStop;
    /**
     * @brief rychlost autobusu
     * 
     */
    qreal speed;

    /**
     * @brief Defaultni konstruktor tridy Autobus
     * 
     */
    Autobus();

    /**
     * @brief Vytvoreni grafickeho objektu obdelniku, predstavujiciho autobus
     * 
     * @return QRectF vytvoreny graficky obdelnik
     */
    QRectF boundingRect() const;

    /**
     * @brief Nakresleni a vybarveni obdelniku, predstavujiciho autobus
     * 
     * @param painter QPainter kreslici objekt
     * @param option QStyleOptionGraphicsItem nastaveni stylu grafickeho objektu
     * @param widget QWidget rodicovsky widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief Prirazuje autobusu linku, po ktere bude jezdit
     * 
     * @param lin Line linka, po ktere ma autobus jezdit
     */
    void setLine(Line lin);

    /**
     * @brief Vraci linku, po ktere autobus jezdi
     * 
     * @return Line linka autobusu
     */
    Line getLine();

    /**
     * @brief Vraci nazev linky, po ktere autobus jezdi
     * 
     * @return string nazev linky autobusu
     */
    string getLineId();

protected:

    /**
     * @brief Stara se o pohyb autobusu a zastavovani na zastavkach na zaklade casu
     * 
     * @param phase int posun
     */
    void advance(int phase);

private:

    bool finish = false;
    list<Coordinate> route;
    qreal angle;
    Coordinate lastC;
    Line line;

    /**
     * @brief Vytvoreni trasy autobusu podle ulic
     * 
     */
    void makeRoute();
};

#endif // AUTOBUS_H
