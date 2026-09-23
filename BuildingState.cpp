//state

#include "BuildingState.h"

#include <string>
#include <iostream>

using namespace std;

//BuildingState
BuildingState::BuildingState(status state) : state(state) {}

status BuildingState::getState() {
    return state;
}

void BuildingState::setBuildingState(BuildingState* state) {
    building->setState(state);
}

//LockedState
LockedState::LockedState(status state) : BuildingState(state) {}

bool LockedState::access(Person* person) {
    if(person->getRole() == Role::SECURITY) {
        cout << person->getName() << "whith role: " << person->getRole() << ", accessed building " << building->getName() << endl;
        return true;
    }

    cout << person->getRole() << ": " << person->getName() << ", attempted to access locked building " << building->getName() << endl;
    return false;
}

void LockedState::lockdown() {
    return;
}

//UnlockedState
UnlockedState::UnlockedState(status state) : BuildingState(state) {}

bool UnlockedState::access(Person* person) {
    cout << person->getName() << "whith role: " << person->getRole() << ", accessed building " << building->getName() << endl;
    return true;
}

void UnlockedState::lockdown() {
    building->setState(new LockedState(status::LOCKED));
}

//CleaningOnlyState
CleaningOnlyState::CleaningOnlyState(status state) : BuildingState(state) {}

bool CleaningOnlyState::access(Person* person) {
    if(person->getRole() == Role::SECURITY || person->getRole() == Role::CLEANINGSTAFF) {
        cout << person->getName() << "whith role: " << person->getRole() << ", accessed building " << building->getName() << endl;
        return true;
    }

    cout << person->getRole() << ": " << person->getName() << ", attempted to access locked building " << building->getName() << endl;
    return false;
}

void CleaningOnlyState::lockdown() {
    building->setState(new LockedState(status::LOCKED));
}

//LecturerOnlyState
LecturerOnlyState::LecturerOnlyState(status state) : BuildingState(state) {}

bool LecturerOnlyState::access(Person* person) {
    if(person->getRole() == Role::SECURITY || person->getRole() == Role::CLEANINGSTAFF || person->getRole() == Role::LECTURER) {
        cout << person->getName() << "whith role: " << person->getRole() << ", accessed building " << building->getName() << endl;
        return true;
    }

    cout << person->getRole() << ": " << person->getName() << ", attempted to access locked building " << building->getName() << endl;
    return false;
}

void LecturerOnlyState::lockdown() {
    building->setState(new LockedState(status::LOCKED));
}

//NoStudentsState
NoStudentsState::NoStudentsState(status state) : BuildingState(state) {}

bool NoStudentsState::access(Person* person) {
    if(person->getRole() == Role::SECURITY || person->getRole() == Role::CLEANINGSTAFF || person->getRole() == Role::LECTURER || person->getRole() == Role::TUTOR) {
        cout << person->getName() << "whith role: " << person->getRole() << ", accessed building " << building->getName() << endl;
        return true;
    }

    cout << person->getRole() << ": " << person->getName() << ", attempted to access locked building " << building->getName() << endl;
    return false;
}

void NoStudentsState::lockdown() {
    building->setState(new LockedState(status::LOCKED));
}