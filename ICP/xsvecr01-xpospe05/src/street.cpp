/**
 * @file street.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida ulice. 
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "street.h"

Street::Street(){

}
Street::Street(string i, list<Coordinate> coords){
    this->id = i;
    for (Coordinate c : coords){
        this->coordinates.push_back(c);
    }
}

string Street::getId(){
    return this->id;
}

list<Coordinate> Street::getCoordinates(){
    return this->coordinates;
}

list<Stop> Street::getStops(){
    return this->stops;
}

Coordinate Street::begin(){
    return this->coordinates.front();
}

Coordinate Street::end(){
    return this->coordinates.back();
}

bool Street::addStop(Stop stop){
    int py = stop.getCoordinate().getY();
    int px = stop.getCoordinate().getX();
    Coordinate c0;
    for(Coordinate c1: this->coordinates){
        if(((c0.getX() != 0) && (c0.getY() != 0)) && ((c0.diffX(c1) != 0) || (c0.diffY(c1) != 0))){
            int x1 = c0.getX();
            int y1 = c0.getY();
            int x2 = c1.getX();
            int y2 = c1.getY();

            if(((x1 >= px && x2 <= px) || (x1 <=px && x2 >= px)) && ((y1 >= py && y2 <= py) || (y1 <=py && y2 >= py))){
                int n[2];
                n[0] = -(y2-y1);
                n[1] = x2-x1;
                int c = -n[0]*x1 -n[1]*y1;
                if((n[0]*px + n[1]*py + c) == 0){
                    stop.setStreet(this);
                    this->stops.push_back(stop);
                    return true;
                }
            }
        }
        c0 = c1;
    }
    return false;
}

bool Street::follows(Street s) {
    if((this->begin().equals(s.begin())) || (this->begin().equals(s.end())) ||
        (this->end().equals(s.end())) || (this->end().equals(s.begin())))
    {
        return true;
    }
    else return false;
}
