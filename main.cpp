#include "Alarm.h"
#include "Building.h"
#include "BuildingState.h"
#include "CampusGuardFacade.h"
#include "DispatchCentre.h"
#include "DispatchStrategy.h"
#include "Incident.h"
#include "IncidentMediator.h"
#include "OperatorCommand.h"
#include "OperatorConsole.h"
#include "Person.h"
#include "ResponseUnit.h"
#include "UnitType.h"

#include <iostream>

using namespace std;

int main()
{
    cout << "CampusGuard - Emergency Response" << endl;

    //~~Setting everything up~~

    //State Pattern - Buildings
    Building engineering("Engineering 1");
    engineering.addAlarm(new AlarmConnector("ENG-MOTION", AlarmType::MOTION));
    engineering.addAlarm(new AlarmConnector("ENG-WINDOW", AlarmType::WINDOW));

    Building examHall("Exam Hall");
    examHall.addAlarm(new AlarmConnector("HALL-FIRE", AlarmType::FIRE));

    //Mediator Pattern - IncidentMediator
    IncidentCoordinator coordinator;

    //Strategy Pattern - DispatchCentre
    DispatchCentre centre(new FastestUnitStrategy());

    //Mediator Pattern - ResponseUnit
    SecurityTeam* alpha = new SecurityTeam("SEC-ALPHA", 3);
    SecurityTeam* bravo = new SecurityTeam("SEC-BRAVO", 7);

    MedicalTeam* medic1 = new MedicalTeam("MED-1", 5);
    MedicalTeam* medic2 = new MedicalTeam("MED-2", 8);

    FacilitiesTeam* facilities = new FacilitiesTeam("FAC-1", 4);

    CommsTeam* comms = new CommsTeam("COMMS", 1);

    ResponseUnit* units[] = { alpha, bravo, medic1, medic2, facilities, comms };
    for(ResponseUnit* unit : units)
    {
        centre.addUnit(unit);
        coordinator.addUnit(unit);
    }

    centre.printRoster();

    //Command pattern - OperatorConsole
    OperatorConsole console("Control Room");

    //Facade pattern - CampusGuardFacade
    CampusGuardFacade guard(&console, &centre, &coordinator);

    //Incidents
    Incident* breakIn = guard.reportIncident("INC-101", "Door forced on the ground", &engineering, UnitType::SECURITY);
    Incident* collapse = guard.reportIncident("INC-202", "Student collapsed during an exam", &examHall, UnitType::MEDICAL);

    //~~Scenario 1~~
    cout << "\n===== Scenario 1 - break in at Engineering 1 =====" << endl;

    //respond
    guard.respondToBreach(breakIn, &engineering, engineering.findAlarm("ENG-MOTION"), new MassResponseStrategy());

    //security on scene reports
    alpha->reportBreach();
    alpha->requestBackup();

    //people arrive
    Person John("John", Role::STUDENT);
    Person Steve("Steve", Role::SECURITY);

    engineering.access(John);
    engineering.access(Steve);

    //Re-arm Building, and try cancel
    console.queueCommand(new SecureBuildingCommand(&engineering));
    console.executePending();
    console.cancelLast();

    //Resolve Break-in
    console.queueCommand(new ResolveIncidentCommand(&coordinator, breakIn));
    console.executePending();

    //dispatch
    console.queueCommand(new DispatchUnitsCommand(&centre, breakIn));
    console.executePending();
    centre.printRoster();

    //~~Scenario 2~~
    cout << "\n===== Scenario 2 - Student collapses at Exam Hall =====" << endl;

    console.queueCommand(new RestrictAccessCommand(&examHall, new NoStudentsState()));
    console.executePending();

    console.queueCommand(new RestrictAccessCommand(&examHall, new LecturerOnlyState()));
    console.executePending();
    console.cancelLast();

    //dispatch fastest medical response
    centre.setStrategy(new FastestUnitStrategy());
    console.queueCommand(new DispatchUnitsCommand(&centre, collapse));
    console.executePending();

    //confirm incident
    medic1->confirmCasualty();

    //check access while medical team at work
    Person Harry("Dr Harry", Role::LECTURER);
    Person Bill("Bill", Role::TUTOR);
    Person Jeff("Jeff", Role::STUDENT);

    examHall.access(Harry);
    examHall.access(Bill);
    examHall.access(Jeff);

    //attempt to sound an alarm that doesnt exist
    console.queueCommand(new SoundAlarmCommand(examHall.findAlarm("nothing")));
    console.executePending();

    //seal hall while moved
    console.queueCommand(new SecureBuildingCommand(&examHall));
    console.executePending();

    //close Incident
    guard.closeIncident(collapse);
    centre.printRoster();

    cout << "\n===== CampusGuard Shutting Down =====\n";

    delete breakIn;
    delete collapse;

    return 0;
}