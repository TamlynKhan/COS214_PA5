#ifndef BUILDINGSTATE_H
#define BUILDINGSTATE_H

#include <string>

class Building;
class Person;

// Transition ownership: lockdown() hands the current state to the new LockedState,
// which resumes to it on reopen(). Any other state returned by a transition is new,
// and the Building deletes the state it replaces.
class BuildingState
{
    public:
        virtual ~BuildingState();

        virtual std::string getName() const = 0;
        virtual bool allows(const Person& person) const = 0;
        virtual bool armsAlarms() const;
        virtual bool acceptsRestriction() const;
        virtual void admit(Building& building, const Person& person) const;
        virtual BuildingState* lockdown();
        virtual BuildingState* reopen();
};

class UnlockedState : public BuildingState
{
    public:
        std::string getName() const override;
        bool allows(const Person& person) const override;
};

class LockedState : public BuildingState
{
    public:
        explicit LockedState(BuildingState* resumeTo = nullptr);
        ~LockedState() override;

        LockedState(const LockedState&) = delete;
        LockedState& operator=(const LockedState&) = delete;

        std::string getName() const override;
        bool allows(const Person& person) const override;
        bool armsAlarms() const override;
        bool acceptsRestriction() const override;
        void admit(Building& building, const Person& person) const override;
        BuildingState* lockdown() override;
        BuildingState* reopen() override;

    private:
        BuildingState* resumeTo;
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
