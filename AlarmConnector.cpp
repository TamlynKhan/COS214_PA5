//Adapter

#include "AlarmConnector.h"

AlarmConnector::AlarmConnector(string name, AlarmType type, bool active) : Alarm(name, type) {
    if(!active) this->alarm = new BlueSecurityAlarm(0);
    if(active) this->alarm = new BlueSecurityAlarm(1);
}

int AlarmConnector::translateCode(AlarmType type) {
    switch(type) {
        case AlarmType::FIRE:
            return 0;
            break;
        case AlarmType::MOTION:
            return 1;
            break;
        case AlarmType::WINDOW:
            return 2;
            break;
        default:
            return -1;
    }
}

void AlarmConnector::activate(AlarmType type) {
    if(alarm->getAlarmStatus() == 0) alarm->setAlarm(translateCode(type), 1);
}

void AlarmConnector::deactivate(AlarmType type) {
    if(alarm->getAlarmStatus() == 1) alarm->setAlarm(translateCode(type), 0);
}

bool AlarmConnector::getState() {
    if(alarm->getAlarmStatus() < 0 || alarm->getAlarmStatus() > 1) return false;
    
    return alarm->getAlarmStatus();
}

AlarmConnector::~AlarmConnector() {
    delete alarm;
}