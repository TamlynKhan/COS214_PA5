#include "Incident.h"
#include "Building.h"

#include <iostream>

Incident::Incident(const std::string& id, const std::string& description, Building* location, UnitType requiredUnit)
    : id(id), description(description), location(location), requiredUnit(requiredUnit), status(IncidentStatus::REPORTED)
{
}

std::string Incident::getId() const
{
    return id;
}

std::string Incident::getDescription() const
{
    return description;
}

Building* Incident::getLocation() const
{
    return location;
}

std::string Incident::getLocationName() const
{
    if (location == nullptr)
    {
        return "an unknown location";
    }
    return location->getName();
}

UnitType Incident::getRequiredUnit() const
{
    return requiredUnit;
}

IncidentStatus Incident::getStatus() const
{
    return status;
}

std::string Incident::getStatusName() const
{
    static const char* const names[] = { "Reported", "Dispatched", "Resolved" };
    return names[static_cast<int>(status)];
}

bool Incident::isResolved() const
{
    return status == IncidentStatus::RESOLVED;
}

void Incident::setStatus(IncidentStatus newStatus)
{
    std::string oldName = getStatusName();
    status = newStatus;
    std::cout << "[Incident " << id << "] status " << oldName << " -> " << getStatusName() << std::endl;
}
