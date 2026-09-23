#include "CommsTeam.h"

CommsTeam::CommsTeam(IncidentMediator* im) :ResponseUnit(im) {}

CommsTeam::~CommsTeam() {}
   
void CommsTeam::issueEvacuation(const std::string& location) {
    std::cout << "The Comms Team has issued an evacuation instruction for " << location << std::endl;
    lastEvent = "EVACUATE:" + location;
}
    
void CommsTeam::sendClear(const std::string& location) {
    std::cout << "The Comms Team sent an all-clear for" << location << std::endl;
    lastEvent = "ALL_CLEAR:" + location;
}

std::string CommsTeam::get() {
    return lastEvent;
}
    
void CommsTeam::set(std::string change) {}