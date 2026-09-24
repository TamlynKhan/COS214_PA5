#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

#include "UnitType.h"

class Building;

enum class IncidentStatus
{
    REPORTED,
    DISPATCHED,
    RESOLVED
};

class Incident
{
    public:
        Incident(const std::string& id, const std::string& description, Building* location, UnitType requiredUnit);

        std::string getId() const;
        std::string getDescription() const;
        Building* getLocation() const;
        std::string getLocationName() const;
        UnitType getRequiredUnit() const;
        IncidentStatus getStatus() const;
        std::string getStatusName() const;
        bool isResolved() const;
        void setStatus(IncidentStatus newStatus);

    private:
        std::string id;
        std::string description;
        Building* location;
        UnitType requiredUnit;
        IncidentStatus status;
};

#endif
