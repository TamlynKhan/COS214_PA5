#include "IncidentCoordinator.h"
#include "ResponseUnit.h"

IncidentCoordinator::IncidentCoordinator() {}
    
IncidentCoordinator::~IncidentCoordinator() {}
    
void IncidentCoordinator::addUnit(ResponseUnit* unit) {
    units.push_back(unit);
}
     
void IncidentCoordinator::notify(ResponseUnit* origin) {
    EventType type = origin->get();
    std::string location = origin->getLocation();

    for (ResponseUnit* unit : units) {
        if (unit != origin) {
            unit->set(type, location);
        }
    }
}

void IncidentCoordinator::resolveIncident(const std::string& location) {
    for (ResponseUnit* unit : units) {
        unit->set(EventType::RESOLVED, location);
    }
}