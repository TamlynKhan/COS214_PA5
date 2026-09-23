#include "MedicalTeam.h"

MedicalTeam::MedicalTeam(IncidentMediator* im) :ResponseUnit(im) {}

MedicalTeam::~MedicalTeam() {}
   
void MedicalTeam::confrimCasualty(const std::string& location) {
    std::cout << "The Medical Team confirmed a casualty at" << location << std::endl;
    lastEvent = "CASUALTY:" + location;
    change();
}
    
void MedicalTeam::standBy() {
    std::cout << "The Medical Team is on standby" << std::endl;
    }

std::string MedicalTeam::get() {
    return lastEvent;
}
    
void MedicalTeam::set(std::string change) {
    
}