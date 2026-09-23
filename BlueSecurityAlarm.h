//Adapter

#ifndef BLUESECURITYALARM_H
#define BLUESECURITYALARM_H

#include <iostream>
#include <string>

using namespace std;

class BlueSecurityAlarm {
    private:
        int code;
        int status;

    public:
        BlueSecurityAlarm(int);

        void setAlarm(int, int);

        int getAlarmCode();
        int getAlarmStatus();
};

#endif