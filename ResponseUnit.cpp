#include "ResponseUnit.h"
#include "Building.h"
#include "Incident.h"

#include <iostream>

ResponseUnit::ResponseUnit(const std::string& callSign, UnitType specialty, int responseMinutes)
    : callSign(callSign), specialty(specialty), responseMinutes(responseMinutes), assignment(nullptr), incidentMediator(nullptr)
{
}

ResponseUnit::~ResponseUnit()
{
}

void ResponseUnit::setMediator(IncidentMediator* incidentMediator)
{
    this->incidentMediator = incidentMediator;
}

std::string ResponseUnit::getCallSign() const
{
    return callSign;
}

UnitType ResponseUnit::getSpecialty() const
{
    return specialty;
}

int ResponseUnit::getResponseMinutes() const
{
    return responseMinutes;
}

Incident* ResponseUnit::getAssignment() const
{
    return assignment;
}

bool ResponseUnit::isAvailable() const
{
    return assignment == nullptr;
}

bool ResponseUnit::canRespondTo(const Incident& incident) const
{
    return isAvailable() && specialty == incident.getRequiredUnit();
}

void ResponseUnit::deploy(Incident* incident)
{
    if (incident == nullptr)
    {
        std::cout << "[" << callSign << "] cannot deploy to an unknown incident" << std::endl;
        return;
    }
    assignment = incident;
    std::cout << "[" << callSign << "] " << unitTypeName(specialty) << " unit deployed to " << incident->getId()
              << " at " << incident->getLocationName() << " (ETA " << responseMinutes << " min)" << std::endl;
    changed(EventType::UNIT_DEPLOYED, incident);
}

void ResponseUnit::standDown()
{
    if (assignment == nullptr)
    {
        return;
    }
    std::cout << "[" << callSign << "] standing down from " << assignment->getId() << std::endl;
    assignment = nullptr;
}

void ResponseUnit::onResponderDeployed(ResponseUnit*, Incident*)
{
}

void ResponseUnit::onBreach(Incident*)
{
}

void ResponseUnit::onCasualty(Incident*)
{
}

void ResponseUnit::onIncidentResolved(Incident*)
{
}

void ResponseUnit::changed(EventType event, Incident* incident)
{
    if (incidentMediator == nullptr)
    {
        std::cout << "[" << callSign << "] is not linked to a coordinator; " << eventTypeName(event) << " not shared" << std::endl;
        return;
    }
    incidentMediator->notify(this, event, incident);
}

SecurityTeam::SecurityTeam(const std::string& callSign, int responseMinutes)
    : ResponseUnit(callSign, UnitType::SECURITY, responseMinutes)
{
}

bool SecurityTeam::reportBreach()
{
    if (getAssignment() == nullptr)
    {
        std::cout << "[" << getCallSign() << "] cannot report a breach while not deployed" << std::endl;
        return false;
    }
    std::cout << "[" << getCallSign() << "] reports a security breach at " << getAssignment()->getLocationName() << std::endl;
    changed(EventType::BREACH, getAssignment());
    return true;
}

bool SecurityTeam::requestBackup()
{
    if (getAssignment() == nullptr)
    {
        std::cout << "[" << getCallSign() << "] cannot request backup while not deployed" << std::endl;
        return false;
    }
    std::cout << "[" << getCallSign() << "] requests backup at " << getAssignment()->getLocationName() << std::endl;
    changed(EventType::BACKUP_REQUESTED, getAssignment());
    return true;
}

void SecurityTeam::onCasualty(Incident* incident)
{
    if (isAvailable())
    {
        deploy(incident);
    }
    std::cout << "[" << getCallSign() << "] cordoning " << incident->getLocationName() << " and escorting paramedics" << std::endl;
}

MedicalTeam::MedicalTeam(const std::string& callSign, int responseMinutes)
    : ResponseUnit(callSign, UnitType::MEDICAL, responseMinutes)
{
}

bool MedicalTeam::confirmCasualty()
{
    if (getAssignment() == nullptr)
    {
        std::cout << "[" << getCallSign() << "] cannot confirm a casualty while not deployed" << std::endl;
        return false;
    }
    std::cout << "[" << getCallSign() << "] confirms a casualty at " << getAssignment()->getLocationName() << std::endl;
    changed(EventType::CASUALTY, getAssignment());
    return true;
}

void MedicalTeam::onBreach(Incident* incident)
{
    std::cout << "[" << getCallSign() << "] moving to standby near " << incident->getLocationName() << " for possible casualties" << std::endl;
}

FacilitiesTeam::FacilitiesTeam(const std::string& callSign, int responseMinutes)
    : ResponseUnit(callSign, UnitType::FACILITIES, responseMinutes)
{
}

void FacilitiesTeam::onBreach(Incident* incident)
{
    if (incident->getLocation() == nullptr)
    {
        return;
    }
    std::cout << "[" << getCallSign() << "] locking down " << incident->getLocationName() << std::endl;
    incident->getLocation()->lockdown();
}

void FacilitiesTeam::onCasualty(Incident* incident)
{
    std::cout << "[" << getCallSign() << "] clearing an access route and holding lifts at " << incident->getLocationName() << std::endl;
}

void FacilitiesTeam::onIncidentResolved(Incident* incident)
{
    if (incident->getLocation() == nullptr)
    {
        return;
    }
    std::cout << "[" << getCallSign() << "] restoring normal access to " << incident->getLocationName() << std::endl;
    incident->getLocation()->reopen();
}

CommsTeam::CommsTeam(const std::string& callSign, int responseMinutes)
    : ResponseUnit(callSign, UnitType::COMMUNICATIONS, responseMinutes)
{
}

void CommsTeam::onResponderDeployed(ResponseUnit* responder, Incident* incident)
{
    std::cout << "[" << getCallSign() << "] notifying occupants of " << incident->getLocationName() << ": "
              << responder->getCallSign() << " is en route, keep access routes clear" << std::endl;
}

void CommsTeam::onBreach(Incident* incident)
{
    std::cout << "[" << getCallSign() << "] alerting campus: shelter in place and avoid " << incident->getLocationName() << std::endl;
}

void CommsTeam::onCasualty(Incident* incident)
{
    std::cout << "[" << getCallSign() << "] notifying the campus clinic of a casualty at " << incident->getLocationName() << std::endl;
}

void CommsTeam::onIncidentResolved(Incident* incident)
{
    std::cout << "[" << getCallSign() << "] sending an all-clear for " << incident->getLocationName() << std::endl;
}
