#include "Alarm.h"
#include "BlueSecurityAlarm.h"

#include <iostream>

std::string alarmTypeName(AlarmType type)
{
    static const char* const names[] = { "Fire", "Window", "Motion" };
    return names[static_cast<int>(type)];
}

Alarm::Alarm(const std::string& name, AlarmType type)
    : name(name), type(type)
{
}

Alarm::~Alarm()
{
}

std::string Alarm::getName() const
{
    return name;
}

AlarmType Alarm::getType() const
{
    return type;
}

AlarmConnector::AlarmConnector(const std::string& name, AlarmType type)
    : Alarm(name, type), legacyAlarm(new BlueSecurityAlarm(0))
{
}

AlarmConnector::~AlarmConnector()
{
    delete legacyAlarm;
}

bool AlarmConnector::activate()
{
    if (isActive())
    {
        std::cout << "[AlarmConnector " << getName() << "] already active" << std::endl;
        return false;
    }
    int code = translateCode(getType());
    std::cout << "[AlarmConnector " << getName() << "] translating " << alarmTypeName(getType()) << " activation to legacy code " << code << std::endl;
    legacyAlarm->setAlarm(code, 1);
    return isActive();
}

bool AlarmConnector::deactivate()
{
    if (!isActive())
    {
        std::cout << "[AlarmConnector " << getName() << "] is not active; nothing to deactivate" << std::endl;
        return false;
    }
    int code = translateCode(getType());
    std::cout << "[AlarmConnector " << getName() << "] translating " << alarmTypeName(getType()) << " deactivation to legacy code " << code << std::endl;
    legacyAlarm->setAlarm(code, 0);
    return !isActive();
}

bool AlarmConnector::isActive() const
{
    return legacyAlarm->getAlarmStatus() == 1;
}

int AlarmConnector::translateCode(AlarmType type) const
{
    switch (type)
    {
        case AlarmType::FIRE:
            return 0;
        case AlarmType::MOTION:
            return 1;
        case AlarmType::WINDOW:
            return 2;
    }
    return -1;
}
