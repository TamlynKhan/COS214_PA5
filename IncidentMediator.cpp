#include "IncidentMediator.h"
#include "Incident.h"
#include "ResponseUnit.h"

#include <iostream>

std::string eventTypeName(EventType type)
{
    static const char* const names[] = { "UNIT DEPLOYED", "UNIT RECALLED", "BREACH", "CASUALTY", "BACKUP REQUESTED" };
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

    if (event == EventType::BACKUP_REQUESTED)
    {
        sendBackup(origin, incident);
        return;
    }

    std::vector<ResponseUnit*> responders = selectResponders(origin, incident);
    switch (event)
    {
        case EventType::UNIT_DEPLOYED:
            for (ResponseUnit* unit : responders)
            {
                unit->onResponderDeployed(origin, incident);
            }
            break;
        case EventType::UNIT_RECALLED:
            for (ResponseUnit* unit : responders)
            {
                unit->onResponderRecalled(origin, incident);
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

Incident* IncidentCoordinator::openIncidentAt(const Building* building, const Incident* except) const
{
    if (building == nullptr)
    {
        return nullptr;
    }
    for (ResponseUnit* unit : units)
    {
        Incident* assignment = unit->getAssignment();
        if (assignment != nullptr && assignment != except && !assignment->isResolved() && assignment->getLocation() == building)
        {
            return assignment;
        }
    }
    return nullptr;
}

// One responder per specialty (other than the origin's), preferring a unit already on this
// incident, then an available unit, then a unit busy elsewhere that can still act remotely.
std::vector<ResponseUnit*> IncidentCoordinator::selectResponders(ResponseUnit* origin, Incident* incident) const
{
    std::vector<ResponseUnit*> selected;
    std::vector<int> ranks;

    for (ResponseUnit* unit : units)
    {
        if (unit == origin || (origin != nullptr && unit->getSpecialty() == origin->getSpecialty()))
        {
            continue;
        }

        int rank = unit->getAssignment() == incident ? 0 : (unit->isAvailable() ? 1 : 2);
        bool placed = false;
        for (std::size_t i = 0; i < selected.size(); ++i)
        {
            if (selected[i]->getSpecialty() == unit->getSpecialty())
            {
                if (rank < ranks[i])
                {
                    selected[i] = unit;
                    ranks[i] = rank;
                }
                placed = true;
                break;
            }
        }
        if (!placed)
        {
            selected.push_back(unit);
            ranks.push_back(rank);
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
