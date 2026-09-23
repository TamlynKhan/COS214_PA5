#include "CommsTeam.h"

CommsTeam::CommsTeam(IncidentMediator* im) : ResponseUnit(im) , lastEvent(EventType::NONE) , lastLocation("") {}

CommsTeam::~CommsTeam() {}
   
void CommsTeam::issueEvacuation(const std::string& location) {
    std::cout << "The Comms Team has issued an evacuation instruction for " << location << std::endl;
    lastEvent = EventType::BREACH; 
    lastLocation = location;
}
    
void CommsTeam::sendAllClear(const std::string& location) {
    std::cout << "The Comms Team sent an all-clear for" << location << std::endl;
    lastEvent = EventType::RESOLVED; 
    lastLocation = location;
}

EventType CommsTeam::get() const{
    return lastEvent;
}

std::string CommsTeam::getLocation() const{
    return lastLocation;
}
    
void CommsTeam::set(EventType type, const std::string& location) {
    if (type == EventType::BREACH || type == EventType::CASUALTY) {
        issueEvacuation(location);
    }
    
    else if (type == EventType::RESOLVED) {
        sendAllClear(location);
    }
}