#include "IncidentCoordinator.h"
#include "ResponseUnit.h"

IncidentCoordinator::IncidentCoordinator() {}
    
IncidentCoordinator::~IncidentCoordinator() {}
    
void IncidentCoordinator::addUnit(ResponseUnit* unit) {
    units.push_back(unit);
}
     
void IncidentCoordinator::notify(ResponseUnit* origin) {

    std::string change = origin->get();
    for (ResponseUnit* unit : units) {
        if (unit != origin) {
            unit->set(change);
        }
    }
}