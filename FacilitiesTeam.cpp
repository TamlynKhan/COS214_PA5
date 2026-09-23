#include "FacilitiesTeam.h"

FacilitiesTeam::FacilitiesTeam(IncidentMediator* im) : ResponseUnit(im) , lastEvent(EventType::NONE) , lastLocation("") {}

FacilitiesTeam::~FacilitiesTeam() {}
   
void FacilitiesTeam::lockArea(const std::string& location) {
    std::cout << "The Facilities Team is locking the following area: " << location << std::endl;
    lastEvent = EventType::BREACH; 
    lastLocation = location;
}
    
void FacilitiesTeam::openEvacuationRoute(const std::string& location) {
    std::cout << "The Facilities Team is opening an evacuation route at" << location << std::endl;
    lastEvent = EventType::CASUALTY; 
    lastLocation = location;
}

EventType FacilitiesTeam::get() const{
    return lastEvent;
}

std::string FacilitiesTeam::getLocation() const{
    return lastLocation;
}
    
void FacilitiesTeam::set(EventType type, const std::string& location) {
    if (type == EventType::BREACH) {
        lockArea(location);
    }
    
    else if (type == EventType::CASUALTY) {
        openEvacuationRoute(location);
    }
}