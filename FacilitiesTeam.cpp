#include "FacilitiesTeam.h"

FacilitiesTeam::FacilitiesTeam(IncidentMediator* im) :ResponseUnit(im) {}

FacilitiesTeam::~FacilitiesTeam() {}
   
std::string FacilitiesTeam::get() {
    return lastEvent;
}
    
void FacilitiesTeam::set(std::string change) {}