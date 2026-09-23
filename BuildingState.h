//state

#ifndef BUILDINGSTATE_H
#define BUILDINGSTATE_H

#include "Person.h"
#include "Building.h"

#include <iostream>
#include <string>

using namespace std;

enum status {
    LOCKED,
    UNLOCKED,
    CLEANINGONLY,
    LECTURERONLY,
    NOSTUDENTS
};

//BuildingState
class BuildingState {
    protected:
        status state;
        Building* building;
    
    public:
        BuildingState(status);
        void setBuildingState(BuildingState* newState);
    
        status getState();
        virtual bool access(Person*) = 0;
        virtual void lockdown() = 0;
};

//LockedState
class LockedState : public BuildingState {
    public:
        LockedState(status state);
    
        status getState();
        virtual bool access(Person*);
        virtual void lockdown();
};

//UnlockedState
class UnlockedState : public BuildingState {
    public:
        UnlockedState(status state);
    
        status getState();
        virtual bool access(Person*);
        virtual void lockdown();
};

//CleaningOnlyState
class CleaningOnlyState : public BuildingState {
    public:
        CleaningOnlyState(status state);
    
        status getState();
        virtual bool access(Person*);
        virtual void lockdown();
};

//LecturerOnlyState
class LecturerOnlyState : public BuildingState {
    public:
        LecturerOnlyState(status state);
    
        status getState();
        virtual bool access(Person*);
        virtual void lockdown();
};

//NoStudentsState
class NoStudentsState : public BuildingState {
    public:
        NoStudentsState(status state);
    
        status getState();
        virtual bool access(Person*);
        virtual void lockdown();
};

#endif