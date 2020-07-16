/**
 * @file coordinate.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida souradnice. 
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "coordinate.h"
#include "math.h"

Coordinate::Coordinate(){
    this->x = 0;
    this->y = 0;
}

Coordinate::Coordinate(int cx, int cy){
    this->x = cx;
    this->y = cy;
}

int Coordinate::getX(){
    return this->x;
}
int Coordinate::getY(){
    return this->y;
}

int Coordinate::diffX(Coordinate c){
    return (this->x - c.getX());
}
int Coordinate::diffY(Coordinate c){
    return (this->y - c.getY());
}

float Coordinate::distance(Coordinate c){
    float result = sqrt(pow(this->diffX(c), 2) + pow(this->diffY(c), 2));
    if (result < 0) result = -result;
    return result;
}

float Coordinate::angle(Coordinate c){
    return ((atan2(this->diffY(c), this->diffX(c)) * 180) / M_PI);
}

bool Coordinate::equals(Coordinate c){
    if((this->getX() == c.getX()) && (this->getY() == c.getY())){
        return true;
    }
    else return false;
}
