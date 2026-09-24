#include "Alarm.h"
#include "Building.h"
#include "BuildingState.h"
#include "DispatchCentre.h"
#include "DispatchStrategy.h"
#include "Incident.h"
#include "IncidentMediator.h"
#include "OperatorCommand.h"
#include "OperatorConsole.h"
#include "Person.h"
#include "ResponseUnit.h"

#include <iostream>
#include <string>

namespace
{
    void banner(const std::string& title)
    {
        std::cout << "\n========== " << title << " ==========" << std::endl;
    }

    // Scenario 1: a break-in at the Engineering building.
    // Command -> Strategy -> Mediator -> State -> Adapter in one flow.
    void breakInAtEngineering(OperatorConsole& console, DispatchCentre& centre, IncidentMediator& coordinator,
                              Building& engineering, Incident& breakIn, SecurityTeam& firstOnScene)
    {
        banner("Scenario 1: break-in at " + engineering.getName());

        console.queueCommand(new DispatchUnitsCommand(&centre, &breakIn));
        console.queueCommand(new SoundAlarmCommand(engineering.findAlarm("ENG-MOTION")));
        console.executePending();

        banner("Security on scene reports a breach");
        firstOnScene.reportBreach();
        firstOnScene.requestBackup();

        banner("People arrive at the locked building");
        Person student("Thabo", Role::STUDENT);
        Person guard("Naledi", Role::SECURITY);
        engineering.access(student);
        engineering.access(guard);

        banner("Operator re-arms the building, then tries to cancel the lockdown");
        console.queueCommand(new SecureBuildingCommand(&engineering));
        console.executePending();
        console.cancelLast();

        banner("Incident resolved");
        console.queueCommand(new ResolveIncidentCommand(&coordinator, &breakIn));
        console.queueCommand(new DispatchUnitsCommand(&centre, &breakIn));
        console.executePending();
        centre.printRoster();
    }

    // Scenario 2: a casualty during an exam in the Exam Hall.
    // Command (incl. cancel) -> State restriction -> Strategy swap -> Mediator escort.
    void casualtyInExamHall(OperatorConsole& console, DispatchCentre& centre, IncidentMediator& coordinator,
                            Building& examHall, Incident& collapse, MedicalTeam& paramedics)
    {
        banner("Scenario 2: casualty during an exam in " + examHall.getName());

        console.queueCommand(new RestrictAccessCommand(&examHall, new NoStudentsState()));
        console.executePending();
        console.queueCommand(new RestrictAccessCommand(&examHall, new LecturerOnlyState()));
        console.executePending();
        console.cancelLast();

        banner("Mass response for a medical emergency");
        centre.setStrategy(new MassResponseStrategy());
        console.queueCommand(new DispatchUnitsCommand(&centre, &collapse));
        console.executePending();

        banner("Paramedics confirm the casualty");
        paramedics.confirmCasualty();

        banner("Access checks while paramedics work");
        Person lecturer("Dr Mokoena", Role::LECTURER);
        Person tutor("Ayesha", Role::TUTOR);
        Person student("Pieter", Role::STUDENT);
        examHall.access(lecturer);
        examHall.access(tutor);
        examHall.access(student);

        banner("Invalid operations are refused");
        console.queueCommand(new SoundAlarmCommand(examHall.findAlarm("NO-SUCH-ALARM")));
        console.executePending();

        banner("Patient stabilised; incident resolved");
        console.queueCommand(new ResolveIncidentCommand(&coordinator, &collapse));
        console.executePending();
        centre.printRoster();
    }
}

int main()
{
    std::cout << "CampusGuard: emergency response coordination" << std::endl;

    // Buildings own their alarms and current state.
    Building engineering("Engineering 1");
    engineering.addAlarm(new AlarmConnector("ENG-MOTION", AlarmType::MOTION));
    engineering.addAlarm(new AlarmConnector("ENG-WINDOW", AlarmType::WINDOW));
    Building examHall("Exam Hall");
    examHall.addAlarm(new AlarmConnector("HALL-FIRE", AlarmType::FIRE));

    // Incidents refer to buildings and are referred to by units and commands,
    // so they are declared after the buildings and before everything else.
    Incident breakIn("INC-101", "Forced door on the ground floor", &engineering, UnitType::SECURITY);
    Incident collapse("INC-202", "Student collapsed during an exam", &examHall, UnitType::MEDICAL);

    // The coordinator links the units but does not own them; the dispatch centre owns
    // the units and its strategy. Both outlive the console, which owns its commands.
    IncidentCoordinator coordinator;
    DispatchCentre centre(new FastestUnitStrategy());

    SecurityTeam* alpha = new SecurityTeam("SEC-ALPHA", 3);
    SecurityTeam* bravo = new SecurityTeam("SEC-BRAVO", 7);
    MedicalTeam* medic1 = new MedicalTeam("MED-1", 5);
    MedicalTeam* medic2 = new MedicalTeam("MED-2", 8);
    FacilitiesTeam* facilities = new FacilitiesTeam("FAC-1", 4);
    CommsTeam* comms = new CommsTeam("COMMS", 1);
    ResponseUnit* units[] = { alpha, bravo, medic1, medic2, facilities, comms };
    for (ResponseUnit* unit : units)
    {
        centre.addUnit(unit);
        coordinator.addUnit(unit);
    }
    centre.printRoster();

    OperatorConsole console("Control Room");

    breakInAtEngineering(console, centre, coordinator, engineering, breakIn, *alpha);
    casualtyInExamHall(console, centre, coordinator, examHall, collapse, *medic1);

    std::cout << "\nCampusGuard shutting down" << std::endl;
    return 0;
}
