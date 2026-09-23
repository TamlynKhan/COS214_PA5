//Adapter

#ifndef ALARM_H
#define ALARM_H

#include <iostream>
#include <string>

#include "BlueSecurityAlarm.h"
#include "Alarm.h"

using namespace std;

enum AlarmType {
    FIRE,
    WINDOW,
    MOTION,
};

class Alarm {
    protected:
        string name;
        AlarmType type;
    
    public:
        Alarm(string, AlarmType);

        virtual void activate(AlarmType) = 0;
        virtual void deactivate(AlarmType) = 0;
        virtual bool getState() = 0;

        virtual ~Alarm();
};

#endif