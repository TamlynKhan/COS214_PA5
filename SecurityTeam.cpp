#include "SecurityTeam.h"

SecurityTeam::SecurityTeam(IncidentMediator* im) :ResponseUnit(im) {}

SecurityTeam::~SecurityTeam() {}
   
std::string SecurityTeam::get() {
    return lastEvent;
}
    
void SecurityTeam::set(std::string change) {}