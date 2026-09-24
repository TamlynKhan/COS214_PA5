#include "CampusGuardFacade.h"
#include "DispatchCentre.h"
#include "DispatchStrategy.h"
#include "Incident.h"
#include "IncidentMediator.h"
#include "OperatorCommand.h"
#include "OperatorConsole.h"
#include <iostream>

CampusGuardFacade::CampusGuardFacade(OperatorConsole* con, DispatchCentre* cen, IncidentMediator* coord) 
    : console(con) , centre(cen), coordinator (coord) {}
    
Incident* CampusGuardFacade::reportIncident(const std::string& id, const std::string& description, Building* building, UnitType requiredUnit) {
    
    if (building == nullptr) {
        std::cout << "CampusGuardFacade cannot report an incident without a building" << std::endl;
        return nullptr;
    }

    Incident* incident = new Incident(id, description, building, requiredUnit);
    std::cout << "CampusGuardFacade registered incident " << incident->getId() << "(" << unitTypeName(requiredUnit) << ") at " << incident->getLocationName() << std::endl;
    return incident;
}

bool CampusGuardFacade::respondToBreach(Incident* incident, Building* building, Alarm* alarm, DispatchStrategy* strategy) {

    if (console == nullptr || centre == nullptr) {
        std::cout << "CampusGuardFacade cannot respond: console or dispatch centre is missing" << std::endl;
        return false;
    } 
    if (incident == nullptr || building == nullptr) {
        std::cout << "CampusGuardFacade cannot respond to breach without an incident or building" << std::endl;
        return false;
    }

    std::cout << "CampusGuardFacade is coordinating a breach responde for incident " << incident->getId() << std::endl;

    if (strategy != nullptr) {
        centre->setStrategy(strategy);
    }

    console->queueCommand(new DispatchUnitsCommand(centre, incident));
    if (alarm != nullptr) {
        console->queueCommand(new SoundAlarmCommand(alarm));
    }
    console->queueCommand(new SecureBuildingCommand(building));

    console->executePending();
    return true;

}
    
bool CampusGuardFacade::closeIncident(Incident* incident) {

    if (console == nullptr || coordinator == nullptr) {
        std::cout << "CampusGuardFacade cannot close incident: console or coordinator is missing" << std::endl;
        return false;
    } 
    if (incident == nullptr) {
        std::cout << "CampusGuardFacade cannot close an unknown incident" << std::endl;
        return false;
    }

    std::cout << "CampusGuardFacade is closing incident" << incident->getId() << std::endl;

    console->queueCommand(new ResolveIncidentCommand(coordinator, incident));
    console->executePending();
    return true;
}