//Adapter

#include "BlueSecurityAlarm.h"

BlueSecurityAlarm::BlueSecurityAlarm(int status) : status(status) {
    code = 0;
}

void BlueSecurityAlarm::setAlarm(int code, int status) {
    if(status < 0 || status > 1) return;
    if(code < 0 || code > 2) return;

    this->code = code;
    this->status = status;
}

int BlueSecurityAlarm::getAlarmCode() {
    return code;
}

int BlueSecurityAlarm::getAlarmStatus() {
    return status;
}