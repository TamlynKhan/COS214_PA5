#include "FacilitiesTeam.h"

FacilitiesTeam::FacilitiesTeam(IncidentMediator* im) :ResponseUnit(im) {}

FacilitiesTeam::~FacilitiesTeam() {}
   
void FacilitiesTeam::lockArea(const std::string& location) {
    std::cout << "The Facilities Team is locking the following area: " << location << std::endl;
    lastEvent = "LOCKED:" + location;
}
    
void FacilitiesTeam::openEvacuationRoute(const std::string& location) {
    std::cout << "The Facilities Team is opening an evacuation route at" << location << std::endl;
    lastEvent = "EVAC_ROUTE:" + location;
}

std::string FacilitiesTeam::get() {
    return lastEvent;
}
    
void FacilitiesTeam::set(std::string change) {}