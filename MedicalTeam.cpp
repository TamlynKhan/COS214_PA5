#include "MedicalTeam.h"

MedicalTeam::MedicalTeam(IncidentMediator* im) : ResponseUnit(im) , lastEvent(EventType::NONE) , lastLocation("") {}

MedicalTeam::~MedicalTeam() {}
   
void MedicalTeam::confrimCasualty(const std::string& location) {
    std::cout << "The Medical Team confirmed a casualty at" << location << std::endl;
    lastEvent = EventType::CASUALTY; 
    lastLocation = location;
    changed();
}
    
void MedicalTeam::standBy() {
    std::cout << "The Medical Team is on standby" << std::endl;
    }

EventType MedicalTeam::get() const{
    return lastEvent;
}

std::string MedicalTeam::getLocation() const{
    return lastLocation;
}
    
void MedicalTeam::set(EventType type, const std::string& location) {
    if (type == EventType::BREACH) {
        std::cout << "The Medical Team is moving to standby near " << location << "due to security breach" << std::endl;
        standBy();
    }
}