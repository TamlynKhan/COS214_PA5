#include "BuildingState.h"
#include "Building.h"
#include "Person.h"

#include <iostream>

BuildingState::~BuildingState()
{
}

void BuildingState::enter(Building&) const
{
}

void BuildingState::admit(Building&, const Person&) const
{
}

BuildingState* BuildingState::lockdown() const
{
    return new LockedState();
}

BuildingState* BuildingState::reopen() const
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

void UnlockedState::enter(Building& building) const
{
    building.deactivateAlarms();
}

std::string LockedState::getName() const
{
    return "Locked";
}

bool LockedState::allows(const Person& person) const
{
    return person.getRole() == Role::SECURITY;
}

void LockedState::enter(Building& building) const
{
    building.activateAlarms();
}

void LockedState::admit(Building& building, const Person& person) const
{
    std::cout << "[Building " << building.getName() << "] " << person.getName() << " disarms the alarms on entry" << std::endl;
    building.deactivateAlarms();
}

BuildingState* LockedState::lockdown() const
{
    return nullptr;
}

BuildingState* LockedState::reopen() const
{
    return new UnlockedState();
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
