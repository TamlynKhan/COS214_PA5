//state

#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
#include <string>
#include <vector>

#include "BuildingState.h"
#include "AlarmConnector.h"

using namespace std;

class Building {
    protected:
        string name;
        BuildingState* status;
        vector<AlarmConnector*> alarms;
    
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

        void addAlarm(AlarmConnector* alarm) {
            alarms.push_back(alarm);
        }

        Alarm* searchAlarms(string name) {
            for(Alarm* alarm : alarms) {
                if(alarm->getName() == name) {
                    return alarm;
                }
            }
        }

        void setAlarms() {
            for(Alarm* alarm : alarms) {
                alarm->activate(alarm->getType());
            }
        }

        void unsetAlarms() {
            for(Alarm* alarm : alarms) {
                alarm->deactivate(alarm->getType());
            }
        }
        
        ~Building() {
            for(Alarm* alarm : alarms) {
                delete alarm;
            }

            delete status;
        }
};

#endif