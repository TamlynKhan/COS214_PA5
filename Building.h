//state

#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
#include <string>

#include "BuildingState.h"

using namespace std;

class Building {
    protected:
        string name;
        BuildingState* status;
    
    public:
        Building(string name) : name(name) {}

        void setState(BuildingState* status) {
            delete status;
            this->status = status;
        }

        string getState() {
            return status->getState() + "";
        }

        string getName() {
            return name;
        }
};

#endif