#include "OperatorCommand.h"
#include "Alarm.h"
#include "Building.h"
#include "DispatchCentre.h"
#include "IncidentMediator.h"

#include <iostream>

OperatorCommand::~OperatorCommand()
{
}

DispatchUnitsCommand::DispatchUnitsCommand(DispatchCentre* centre, Incident* incident)
    : centre(centre), incident(incident), previousStatus(IncidentStatus::REPORTED)
{
}

bool DispatchUnitsCommand::execute()
{
    if (centre == nullptr || incident == nullptr)
    {
        std::cout << "[DispatchUnitsCommand] cannot dispatch without a dispatch centre and an incident" << std::endl;
        return false;
    }
    previousStatus = incident->getStatus();
    dispatched = centre->dispatch(incident);
    return !dispatched.empty();
}

bool DispatchUnitsCommand::undo()
{
    if (dispatched.empty())
    {
        std::cout << "[DispatchUnitsCommand] no dispatched units to recall" << std::endl;
        return false;
    }
    if (incident->isResolved())
    {
        std::cout << "[DispatchUnitsCommand] " << incident->getId() << " is already resolved; dispatch can no longer be cancelled" << std::endl;
        return false;
    }
    int recalled = centre->recall(incident, dispatched);
    dispatched.clear();
    std::cout << "[DispatchUnitsCommand] recalled " << recalled << " unit(s) from " << incident->getId() << std::endl;
    incident->setStatus(previousStatus);
    return true;
}

std::string DispatchUnitsCommand::describe() const
{
    if (incident == nullptr)
    {
        return "Dispatch units (no incident)";
    }
    return "Dispatch units to " + incident->getId();
}

SecureBuildingCommand::SecureBuildingCommand(Building* building)
    : building(building)
{
}

bool SecureBuildingCommand::execute()
{
    if (building == nullptr)
    {
        std::cout << "[SecureBuildingCommand] cannot secure an unknown building" << std::endl;
        return false;
    }
    std::cout << "[SecureBuildingCommand] requesting lockdown of " << building->getName() << std::endl;
    return building->lockdown();
}

bool SecureBuildingCommand::undo()
{
    std::cout << "[SecureBuildingCommand] lockdown of " << building->getName()
              << " cannot be cancelled from the console; it must be lifted by campus security on site" << std::endl;
    return false;
}

std::string SecureBuildingCommand::describe() const
{
    if (building == nullptr)
    {
        return "Secure building (no building)";
    }
    return "Secure building " + building->getName();
}

SoundAlarmCommand::SoundAlarmCommand(Alarm* alarm)
    : alarm(alarm)
{
}

bool SoundAlarmCommand::execute()
{
    if (alarm == nullptr)
    {
        std::cout << "[SoundAlarmCommand] no alarm system connected" << std::endl;
        return false;
    }
    return alarm->activate();
}

bool SoundAlarmCommand::undo()
{
    if (alarm == nullptr)
    {
        return false;
    }
    return alarm->deactivate();
}

std::string SoundAlarmCommand::describe() const
{
    if (alarm == nullptr)
    {
        return "Sound alarm (no alarm)";
    }
    return "Sound " + alarmTypeName(alarm->getType()) + " alarm " + alarm->getName();
}

ResolveIncidentCommand::ResolveIncidentCommand(IncidentMediator* coordinator, Incident* incident)
    : coordinator(coordinator), incident(incident)
{
}

bool ResolveIncidentCommand::execute()
{
    if (coordinator == nullptr)
    {
        std::cout << "[ResolveIncidentCommand] no incident coordinator available" << std::endl;
        return false;
    }
    return coordinator->resolveIncident(incident);
}

bool ResolveIncidentCommand::undo()
{
    std::cout << "[ResolveIncidentCommand] a resolved incident cannot be reopened; report a new incident instead" << std::endl;
    return false;
}

std::string ResolveIncidentCommand::describe() const
{
    if (incident == nullptr)
    {
        return "Resolve incident (no incident)";
    }
    return "Resolve " + incident->getId();
}
