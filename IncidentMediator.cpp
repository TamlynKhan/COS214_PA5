#include "IncidentMediator.h"
#include "Incident.h"
#include "ResponseUnit.h"

#include <algorithm>
#include <iostream>

std::string eventTypeName(EventType type)
{
    static const char* const names[] = { "UNIT DEPLOYED", "BREACH", "CASUALTY", "BACKUP REQUESTED" };
    return names[static_cast<int>(type)];
}

IncidentMediator::~IncidentMediator()
{
}

IncidentCoordinator::IncidentCoordinator()
{
}

IncidentCoordinator::~IncidentCoordinator()
{
}

void IncidentCoordinator::addUnit(ResponseUnit* unit)
{
    if (unit == nullptr)
    {
        std::cout << "[IncidentCoordinator] refused to register an empty unit" << std::endl;
        return;
    }
    units.push_back(unit);
    unit->setMediator(this);
}

void IncidentCoordinator::notify(ResponseUnit* origin, EventType event, Incident* incident)
{
    if (origin == nullptr || incident == nullptr)
    {
        std::cout << "[IncidentCoordinator] ignored " << eventTypeName(event) << " with no origin unit or incident" << std::endl;
        return;
    }

    std::cout << "[IncidentCoordinator] " << eventTypeName(event) << " from " << origin->getCallSign()
              << " at " << incident->getId() << " (" << incident->getLocationName() << ")" << std::endl;

    std::vector<ResponseUnit*> responders = selectResponders(origin, incident);
    switch (event)
    {
        case EventType::UNIT_DEPLOYED:
            for (ResponseUnit* unit : responders)
            {
                unit->onResponderDeployed(origin, incident);
            }
            break;
        case EventType::BREACH:
            for (ResponseUnit* unit : responders)
            {
                unit->onBreach(incident);
            }
            break;
        case EventType::CASUALTY:
            for (ResponseUnit* unit : responders)
            {
                unit->onCasualty(incident);
            }
            break;
        case EventType::BACKUP_REQUESTED:
            sendBackup(origin, incident);
            break;
    }
}

bool IncidentCoordinator::resolveIncident(Incident* incident)
{
    if (incident == nullptr)
    {
        std::cout << "[IncidentCoordinator] cannot resolve an unknown incident" << std::endl;
        return false;
    }
    if (incident->isResolved())
    {
        std::cout << "[IncidentCoordinator] " << incident->getId() << " is already resolved" << std::endl;
        return false;
    }

    std::cout << "[IncidentCoordinator] resolving " << incident->getId() << " at " << incident->getLocationName() << std::endl;
    for (ResponseUnit* unit : units)
    {
        if (unit->getAssignment() == incident)
        {
            unit->standDown();
        }
    }
    incident->setStatus(IncidentStatus::RESOLVED);

    std::vector<ResponseUnit*> responders = selectResponders(nullptr, incident);
    for (ResponseUnit* unit : responders)
    {
        unit->onIncidentResolved(incident);
    }
    return true;
}

std::vector<ResponseUnit*> IncidentCoordinator::selectResponders(ResponseUnit* origin, Incident* incident) const
{
    std::vector<ResponseUnit*> selected;
    std::vector<UnitType> covered;
    if (origin != nullptr)
    {
        covered.push_back(origin->getSpecialty());
    }

    for (ResponseUnit* unit : units)
    {
        bool reachable = unit->isAvailable() || unit->getAssignment() == incident;
        bool alreadyCovered = std::find(covered.begin(), covered.end(), unit->getSpecialty()) != covered.end();
        if (unit != origin && reachable && !alreadyCovered)
        {
            covered.push_back(unit->getSpecialty());
            selected.push_back(unit);
        }
    }
    return selected;
}

void IncidentCoordinator::sendBackup(ResponseUnit* origin, Incident* incident)
{
    for (ResponseUnit* unit : units)
    {
        if (unit != origin && unit->getSpecialty() == origin->getSpecialty() && unit->isAvailable())
        {
            std::cout << "[IncidentCoordinator] sending " << unit->getCallSign() << " as backup for " << origin->getCallSign() << std::endl;
            unit->deploy(incident);
            return;
        }
    }
    std::cout << "[IncidentCoordinator] no " << unitTypeName(origin->getSpecialty()) << " units available as backup for "
              << incident->getId() << "; escalating to off-campus services" << std::endl;
}
