/**
 * @file stop.cpp
 * @author Jan Pospech [xpospe05]
 * @author Radek Svec [xsvecr01]
 * @brief Trida zastavky.
 * @version 0.1
 * @date 2020-05-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "stop.h"
#include "street.h"
#include <string>

Stop::Stop(){

}

Stop::Stop(string id, Coordinate c){
    this->id = id;
    this->coordinate = c;
}

void Stop::setStreet(Street* s){
    this->street = s;
}

Street* Stop::getStreet(){
    return this->street;
}

Coordinate Stop::getCoordinate(){
    return this->coordinate;
}

string Stop::getId(){
    return this->id;
}

