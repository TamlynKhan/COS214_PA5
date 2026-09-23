#include "MedicalTeam.h"

MedicalTeam::MedicalTeam(IncidentMediator* im) :ResponseUnit(im) {}

MedicalTeam::~MedicalTeam() {}
   
std::string MedicalTeam::get() {
    return lastEvent;
}
    
void MedicalTeam::set(std::string change) {}