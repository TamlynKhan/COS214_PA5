#include "DispatchCentre.h"
#include "DispatchStrategy.h"
#include "Incident.h"
#include "ResponseUnit.h"

#include <iostream>

DispatchCentre::DispatchCentre(DispatchStrategy* strategy)
    : strategy(strategy)
{
}

DispatchCentre::~DispatchCentre()
{
    for (ResponseUnit* unit : roster)
    {
        delete unit;
    }
    delete strategy;
}

void DispatchCentre::addUnit(ResponseUnit* unit)
{
    if (unit == nullptr)
    {
        std::cout << "[DispatchCentre] refused to register an empty unit" << std::endl;
        return;
    }
    roster.push_back(unit);
}

void DispatchCentre::setStrategy(DispatchStrategy* newStrategy)
{
    if (newStrategy == nullptr)
    {
        std::cout << "[DispatchCentre] refused empty dispatch strategy; keeping current strategy" << std::endl;
        return;
    }
    if (newStrategy == strategy)
    {
        return;
    }
    delete strategy;
    strategy = newStrategy;
    std::cout << "[DispatchCentre] dispatch strategy set to " << strategy->getName() << std::endl;
}

std::vector<ResponseUnit*> DispatchCentre::dispatch(Incident* incident)
{
    std::vector<ResponseUnit*> selected;
    if (incident == nullptr)
    {
        std::cout << "[DispatchCentre] dispatch refused: no incident supplied" << std::endl;
        return selected;
    }
    if (incident->isResolved())
    {
        std::cout << "[DispatchCentre] dispatch refused: " << incident->getId() << " is already resolved" << std::endl;
        return selected;
    }
    if (strategy == nullptr)
    {
        std::cout << "[DispatchCentre] dispatch refused: no dispatch strategy configured" << std::endl;
        return selected;
    }

    std::cout << "[DispatchCentre] " << incident->getId() << " requires " << unitTypeName(incident->getRequiredUnit())
              << " units, applying " << strategy->getName() << " strategy" << std::endl;

    selected = strategy->selectUnits(*incident, roster);
    if (selected.empty())
    {
        std::cout << "[DispatchCentre] no available " << unitTypeName(incident->getRequiredUnit())
                  << " units for " << incident->getId() << std::endl;
        return selected;
    }

    for (ResponseUnit* unit : selected)
    {
        unit->deploy(incident);
    }
    incident->setStatus(IncidentStatus::DISPATCHED);
    return selected;
}

int DispatchCentre::recall(Incident* incident, const std::vector<ResponseUnit*>& units)
{
    int recalled = 0;
    for (ResponseUnit* unit : units)
    {
        if (unit->getAssignment() == incident)
        {
            unit->standDown();
            ++recalled;
        }
    }
    return recalled;
}

void DispatchCentre::printRoster() const
{
    std::cout << "[DispatchCentre] roster:" << std::endl;
    for (ResponseUnit* unit : roster)
    {
        std::cout << "    " << unit->getCallSign() << " (" << unitTypeName(unit->getSpecialty()) << ", "
                  << unit->getResponseMinutes() << " min) ";
        if (unit->isAvailable())
        {
            std::cout << "available" << std::endl;
        }
        else
        {
            std::cout << "assigned to " << unit->getAssignment()->getId() << std::endl;
        }
    }
}
