#include "SecurityTeam.h"

SecurityTeam::SecurityTeam(IncidentMediator* im) : ResponseUnit(im) , lastEvent(EventType::NONE) , lastLocation("") {}

SecurityTeam::~SecurityTeam() {}
   
void SecurityTeam::reportBreach(const std::string& location) {
    std::cout << "The Security Team has detected a breach at" << location << std::endl;
    lastEvent = EventType::BREACH; 
    lastLocation = location;
    changed();
}
    
void SecurityTeam::requestBackup() {
    std::cout << "The Security Team is requesting backup" << std::endl;
    lastEvent = EventType::BACKUP_REQUESTED; 
    lastLocation = "";
    changed();
}

EventType SecurityTeam::get() const {
    return lastEvent;
}

std::string SecurityTeam::getLocation() const {
    return lastLocation;
}
    
void SecurityTeam::set(EventType type, const std::string& location) {
    if (type == EventType::CASUALTY) {
        std::cout << "The Security Team is dispatching backup to " << location << "due to casualty report" << std::endl;
    }
}