/**
 * @file autobus.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida autobusu. 
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "autobus.h"
#include "coordinate.h"

#include <QTime>
#include <QTimer>
#include <QCoreApplication>
#include <QEventLoop>

Autobus::Autobus(){
    speed = 1;
    QBrush brush(Qt::red);
    Brush = brush;
}

void Autobus::makeRoute(){
    list<Coordinate> coords;
    int i = 0;
    Street prevStre;
    for(Street stre : this->line.getStreets()){
        if(i == 0){
            prevStre = stre;
            i++;
            continue;
        }
        if(prevStre.end().equals(stre.begin()) || prevStre.end().equals(stre.end())){
            for(Coordinate c : prevStre.getCoordinates()){
                coords.push_back(c);
            }
        }
        else if(prevStre.begin().equals(stre.begin()) || prevStre.begin().equals(stre.end())){
            list<Coordinate> reversed;
            for(Coordinate c : prevStre.getCoordinates()){
                reversed.push_front(c);
            }
            for(Coordinate c : reversed){
                coords.push_back(c);
            }
        }
        prevStre = stre;
    }

    if(prevStre.end().equals(coords.front()) || prevStre.end().equals(coords.front())){
        for(Coordinate c : prevStre.getCoordinates()){
            coords.push_back(c);
        }
    }
    else if(prevStre.begin().equals(coords.front()) || prevStre.begin().equals(coords.front())){
        list<Coordinate> reversed;
        for(Coordinate c : prevStre.getCoordinates()){
            reversed.push_front(c);
        }
        for(Coordinate c : reversed){
            coords.push_back(c);
        }
    }

    list<Coordinate> nodup;
    for(Coordinate c : coords){
        int isin = 0;
        for(Coordinate cn : nodup){
            if(c.equals(cn)){
                isin = 1;
            }
        }
        if(isin == 0){
            nodup.push_back(c);
        }
    }

    Coordinate first = nodup.front();
    nodup.pop_front();
    nodup.push_back(first);

    nodup.push_back(this->line.getStops().front().getCoordinate());

    this->route = nodup;
}


QRectF Autobus::boundingRect() const{
    return QRect(-1,-1,4,3);
}

void Autobus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rec = boundingRect();
    painter->fillRect(rec, Brush);
    painter->drawRect(rec);
}

void Autobus::advance(int phase){
    if(!phase) return;

    Coordinate location(this->pos().x(), this->pos().y());

    for(Stop current: line.getStops()){
        if(((current.getCoordinate().diffX(location) >= -1) &&
            (current.getCoordinate().diffX(location) <= 1)) &&
                ((current.getCoordinate().diffY(location) <= 1) &&
                (current.getCoordinate().diffY(location) >= -1))
            ){
            if(lastStop.getId().compare(current.getId()) != 0){
                if(lastStop.getId().compare(line.getStops().back().getId()) == 0){
                    finish = true;
                }
                else{
                    lastC = location;
                }
                lastStop = current;
                speed = (speed/20);
            }
        }
    }

    if(finish) {
        finish = false;
        Coordinate startStop = line.getStops().front().getCoordinate();
        setPos(startStop.getX(), startStop.getY());

        Coordinate location(this->pos().x(), this->pos().y());
        lastC = location;
        angle = location.angle(route.front());
        setRotation(angle);
        speed = (speed/2);
    }

    if(location.distance(lastC) > 2){
        speed = 1;
    }

    int a = 0;
    Coordinate cprev;
    for(Coordinate c : route){
        if(a == 0) {
            a++;
            cprev = c;
            continue;
        }
        if(((location.diffX(cprev)) == 0) &&
        (((location.diffY(cprev)) >= -1) && (location.diffY(cprev) <= 1)))
        {
            angle = location.angle(c);
            setRotation(angle);
        }
        cprev = c;
    }
    setPos(mapToParent(-(speed), 0));
}

void Autobus::setLine(Line lin){
    this->line = lin;
    this->id = line.getId();
    makeRoute();

    Coordinate startStop = line.getStops().front().getCoordinate();
    setPos(mapToParent(startStop.getX(), startStop.getY()));

    Coordinate location(this->pos().x(), this->pos().y());
    lastStop = line.getStops().front();
    angle = location.angle(route.front());
    setRotation(angle);
}

Line Autobus::getLine(){
    return this->line;
}

string Autobus::getLineId(){
    return this->line.getId();
}

