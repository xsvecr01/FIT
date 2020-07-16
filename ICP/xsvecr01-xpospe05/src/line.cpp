/**
 * @file line.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida linky.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "line.h"
#include <string>

Line::Line(){
    this->id = "undefined";
}

Line::Line(string i){
    this->id = i;
}

bool Line::addStreet(Street street, Stop stop){
    if(this->streets.size() == 0 || this->streets.back().follows(street)){
        if(stop.getStreet()->getId().compare(street.getId()) == 0){
            this->streets.push_back(street);
            this->stops.push_back(stop);
            return true;
        }
    }
    return false;
}

bool Line::addStreet(Street street){
    if(this->streets.size() == 0 || this->streets.back().follows(street)){
        this->streets.push_back(street);
        return true;
    }
    else return false;
}

list<Street> Line::getStreets(){
    return this->streets;
}

list<Stop> Line::getStops(){
    return this->stops;
}

string Line::getId(){
    return this->id;
}
