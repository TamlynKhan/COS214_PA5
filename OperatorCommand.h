#ifndef OPERATORCOMMAND_H
#define OPERATORCOMMAND_H

#include <string>
#include <vector>

#include "Incident.h"

class Alarm;
class Building;
class BuildingState;
class DispatchCentre;
class IncidentMediator;
class ResponseUnit;

class OperatorCommand
{
    public:
        virtual ~OperatorCommand();

        virtual bool execute() = 0;
        virtual bool undo() = 0;
        virtual std::string describe() const = 0;
};

class DispatchUnitsCommand : public OperatorCommand
{
    public:
        DispatchUnitsCommand(DispatchCentre* centre, Incident* incident);

        bool execute() override;
        bool undo() override;
        std::string describe() const override;

    private:
        DispatchCentre* centre;
        Incident* incident;
        IncidentStatus previousStatus;
        std::vector<ResponseUnit*> dispatched;
};

class SecureBuildingCommand : public OperatorCommand
{
    public:
        explicit SecureBuildingCommand(Building* building);

        bool execute() override;
        bool undo() override;
        std::string describe() const override;

    private:
        Building* building;
};

class RestrictAccessCommand : public OperatorCommand
{
    public:
        RestrictAccessCommand(Building* building, BuildingState* policy);
        ~RestrictAccessCommand() override;

        RestrictAccessCommand(const RestrictAccessCommand&) = delete;
        RestrictAccessCommand& operator=(const RestrictAccessCommand&) = delete;

        bool execute() override;
        bool undo() override;
        std::string describe() const override;

    private:
        Building* building;
        // owned: the requested policy until execute(), then the policy it replaced (for undo)
        BuildingState* held;
        std::string policyName;
};

class SoundAlarmCommand : public OperatorCommand
{
    public:
        explicit SoundAlarmCommand(Alarm* alarm);

        bool execute() override;
        bool undo() override;
        std::string describe() const override;

    private:
        Alarm* alarm;
};

class ResolveIncidentCommand : public OperatorCommand
{
    public:
        ResolveIncidentCommand(IncidentMediator* coordinator, Incident* incident);

        bool execute() override;
        bool undo() override;
        std::string describe() const override;

    private:
        IncidentMediator* coordinator;
        Incident* incident;
};

#endif
