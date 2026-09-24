#include "BuildingState.h"
#include "Building.h"
#include "Person.h"

#include <iostream>

BuildingState::~BuildingState()
{
}

bool BuildingState::armsAlarms() const
{
    return false;
}

bool BuildingState::acceptsRestriction() const
{
    return true;
}

void BuildingState::admit(Building&, const Person&) const
{
}

BuildingState* BuildingState::lockdown()
{
    return new LockedState(this);
}

BuildingState* BuildingState::reopen()
{
    return nullptr;
}

std::string UnlockedState::getName() const
{
    return "Unlocked";
}

bool UnlockedState::allows(const Person&) const
{
    return true;
}

LockedState::LockedState(BuildingState* resumeTo)
    : resumeTo(resumeTo)
{
}

LockedState::~LockedState()
{
    delete resumeTo;
}

std::string LockedState::getName() const
{
    return "Locked";
}

bool LockedState::allows(const Person& person) const
{
    return person.getRole() == Role::SECURITY;
}

bool LockedState::armsAlarms() const
{
    return true;
}

bool LockedState::acceptsRestriction() const
{
    return false;
}

void LockedState::admit(Building& building, const Person& person) const
{
    std::cout << "[Building " << building.getName() << "] " << person.getName() << " disarms the alarms on entry" << std::endl;
    building.deactivateAlarms();
}

BuildingState* LockedState::lockdown()
{
    return nullptr;
}

BuildingState* LockedState::reopen()
{
    BuildingState* next = resumeTo;
    resumeTo = nullptr;
    if (next == nullptr)
    {
        next = new UnlockedState();
    }
    return next;
}

std::string CleaningOnlyState::getName() const
{
    return "Cleaning Only";
}

bool CleaningOnlyState::allows(const Person& person) const
{
    return person.getRole() == Role::SECURITY || person.getRole() == Role::CLEANING_STAFF;
}

std::string LecturerOnlyState::getName() const
{
    return "Lecturer Only";
}

bool LecturerOnlyState::allows(const Person& person) const
{
    return person.getRole() == Role::SECURITY || person.getRole() == Role::CLEANING_STAFF || person.getRole() == Role::LECTURER;
}

std::string NoStudentsState::getName() const
{
    return "No Students";
}

bool NoStudentsState::allows(const Person& person) const
{
    return person.getRole() != Role::STUDENT;
}
