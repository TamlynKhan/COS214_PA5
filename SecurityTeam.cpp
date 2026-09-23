#include "SecurityTeam.h"

SecurityTeam::SecurityTeam(IncidentMediator* im) :ResponseUnit(im) {}

SecurityTeam::~SecurityTeam() {}
   
void SecurityTeam::reportBreach(const std::string& location) {
    std::cout << "The Security Team has detected a breach at" << location << std::endl;
    lastEvent = "BREACH:" + location;
    change();
}
    
void SecurityTeam::requestBackup(const std::string& location) {
    std::cout << "The Security Team is requesting backup" << location << std::endl;
    lastEvent = "BACKUP_REQUESTED";
    change();
}

std::string SecurityTeam::get() {
    return lastEvent;
}
    
void SecurityTeam::set(std::string change) {}