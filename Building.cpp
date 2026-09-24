#include "Building.h"
#include "Alarm.h"
#include "BuildingState.h"
#include "Person.h"

#include <iostream>

Building::Building(const std::string& name, BuildingState* initialState)
    : name(name), state(initialState)
{
    if (state == nullptr)
    {
        state = new UnlockedState();
    }
}

Building::~Building()
{
    for (Alarm* alarm : alarms)
    {
        delete alarm;
    }
    delete state;
}

std::string Building::getName() const
{
    return name;
}

std::string Building::getStateName() const
{
    return state->getName();
}

void Building::addAlarm(Alarm* alarm)
{
    if (alarm == nullptr)
    {
        std::cout << "[Building " << name << "] refused to install an empty alarm" << std::endl;
        return;
    }
    alarms.push_back(alarm);
}

Alarm* Building::findAlarm(const std::string& alarmName) const
{
    for (Alarm* alarm : alarms)
    {
        if (alarm->getName() == alarmName)
        {
            return alarm;
        }
    }
    std::cout << "[Building " << name << "] has no alarm named " << alarmName << std::endl;
    return nullptr;
}

void Building::activateAlarms()
{
    for (Alarm* alarm : alarms)
    {
        if (!alarm->isActive())
        {
            alarm->activate();
        }
    }
}

void Building::deactivateAlarms()
{
    for (Alarm* alarm : alarms)
    {
        if (alarm->isActive())
        {
            alarm->deactivate();
        }
    }
}

bool Building::access(const Person& person)
{
    bool allowed = state->allows(person);
    std::cout << "[Building " << name << "] " << roleName(person.getRole()) << " " << person.getName()
              << (allowed ? " GRANTED" : " DENIED") << " access (" << state->getName() << ")" << std::endl;
    if (allowed)
    {
        state->admit(*this, person);
    }
    return allowed;
}

bool Building::lockdown()
{
    BuildingState* next = state->lockdown();
    if (next == nullptr)
    {
        std::cout << "[Building " << name << "] already under lockdown" << std::endl;
        return false;
    }
    changeState(next);
    return true;
}

bool Building::reopen()
{
    BuildingState* next = state->reopen();
    if (next == nullptr)
    {
        std::cout << "[Building " << name << "] no lockdown to lift (" << state->getName() << ")" << std::endl;
        return false;
    }
    changeState(next);
    return true;
}

void Building::changeState(BuildingState* next)
{
    std::cout << "[Building " << name << "] " << state->getName() << " -> " << next->getName() << std::endl;
    delete state;
    state = next;
    state->enter(*this);
}
