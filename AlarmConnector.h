//Adapter

#ifndef ALARMCONNECTOR_H
#define ALARMCONNECTOR_H

#include <iostream>
#include <string>

#include "BlueSecurityAlarm.h"
#include "Alarm.h"

using namespace std;

class AlarmConnector : public Alarm {
    protected:
        BlueSecurityAlarm* alarm;
    
    public:
        AlarmConnector(string, AlarmType, bool);

        void activate(AlarmType);
        void deactivate(AlarmType);
        int translateCode(AlarmType);
        bool getState();

        ~AlarmConnector();
};

#endif