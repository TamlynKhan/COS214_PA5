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
    if (state->armsAlarms() && !alarm->isActive())
    {
        alarm->activate();
    }
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

int Building::activateAlarms()
{
    int changed = 0;
    for (Alarm* alarm : alarms)
    {
        if (!alarm->isActive() && alarm->activate())
        {
            ++changed;
        }
    }
    return changed;
}

int Building::deactivateAlarms()
{
    int changed = 0;
    for (Alarm* alarm : alarms)
    {
        if (alarm->isActive() && alarm->deactivate())
        {
            ++changed;
        }
    }
    return changed;
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
        int rearmed = activateAlarms();
        if (rearmed == 0)
        {
            std::cout << "[Building " << name << "] already under lockdown" << std::endl;
            return false;
        }
        std::cout << "[Building " << name << "] already under lockdown; re-armed " << rearmed << " alarm(s)" << std::endl;
        return true;
    }
    // the new LockedState has taken over the old state so it can resume to it
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
    BuildingState* previous = state;
    changeState(next);
    delete previous;
    return true;
}

BuildingState* Building::restrictAccess(BuildingState* policy)
{
    if (policy == nullptr)
    {
        std::cout << "[Building " << name << "] refused an empty access policy" << std::endl;
        return nullptr;
    }
    if (!state->acceptsRestriction())
    {
        std::cout << "[Building " << name << "] cannot change access to " << policy->getName()
                  << " while " << state->getName() << "; lift the lockdown first" << std::endl;
        return nullptr;
    }
    BuildingState* previous = state;
    changeState(policy);
    return previous;
}

void Building::changeState(BuildingState* next)
{
    std::cout << "[Building " << name << "] " << state->getName() << " -> " << next->getName() << std::endl;
    state = next;
    syncAlarms();
}

void Building::syncAlarms()
{
    if (state->armsAlarms())
    {
        activateAlarms();
    }
    else
    {
        deactivateAlarms();
    }
}
