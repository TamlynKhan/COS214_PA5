#include "BlueSecurityAlarm.h"

#include <iostream>

BlueSecurityAlarm::BlueSecurityAlarm(int status)
    : code(0), status(status)
{
}

void BlueSecurityAlarm::setAlarm(int code, int status)
{
    if (status < 0 || status > 1)
    {
        return;
    }
    if (code < 0 || code > 2)
    {
        return;
    }

    this->code = code;
    this->status = status;
    std::cout << "[BlueSecurityAlarm] legacy panel set to code " << code << ", status " << status << std::endl;
}

int BlueSecurityAlarm::getAlarmCode()
{
    return code;
}

int BlueSecurityAlarm::getAlarmStatus()
{
    return status;
}
