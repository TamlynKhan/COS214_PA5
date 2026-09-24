#ifndef BLUESECURITYALARM_H
#define BLUESECURITYALARM_H

class BlueSecurityAlarm
{
    public:
        explicit BlueSecurityAlarm(int status);

        void setAlarm(int code, int status);

        int getAlarmCode();
        int getAlarmStatus();

    private:
        int code;
        int status;
};

#endif
