#ifndef BUILDINGSTATE_H
#define BUILDINGSTATE_H

#include <string>

class Building;
class Person;

class BuildingState
{
    public:
        virtual ~BuildingState();

        virtual std::string getName() const = 0;
        virtual bool allows(const Person& person) const = 0;
        virtual void enter(Building& building) const;
        virtual void admit(Building& building, const Person& person) const;
        virtual BuildingState* lockdown() const;
        virtual BuildingState* reopen() const;
};

class UnlockedState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
        void enter(Building& building) const override;
};

class LockedState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
        void enter(Building& building) const override;
        void admit(Building& building, const Person& person) const override;
        BuildingState* lockdown() const override;
        BuildingState* reopen() const override;
};

class CleaningOnlyState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
};

class LecturerOnlyState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
};

class NoStudentsState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
};

#endif
