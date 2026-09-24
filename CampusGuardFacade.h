#ifndef CAMPUSGUARDFACADE_H
#define CAMPUSGUARDFACADE_H

class OperatorConsole;
class DispatchCentre;
class DispatchStrategy;
class Incident;
class IncidentMediator;
class Building;
class Alarm;

class CampusGuardFacade {
private:
    OperatorConsole* console;
    DispatchCentre* centre;
    IncidentMediator* coordinator;
public:
    CampusGuardFacade(OperatorConsole* console, DispatchCentre* centre, IncidentMediator* coordinator);
    bool respondToBreach(Incident* incident, Building* building, Alarm* alarm, DispatchStrategy* strategy = nullptr);
    bool closeIncident(Incident* incident);
};
#endif 