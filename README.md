# CampusGuard

Emergency-response coordination for a university campus (COS 214 Practical 5).
Operators report incidents, dispatch security, medical, facilities and comms units,
control building access, and sound alarms on a legacy alarm panel.

Patterns: Command, Mediator, Adapter, Facade, plus State and Strategy.

## Run it (Docker)

The demonstration is launched with:

```
docker compose up --build
```

This builds the image (Ubuntu 24.04, g++, make, valgrind, gdb), compiles the project
with `make`, and runs both scenarios.

Memory check and debugger inside the container:

```
docker compose run --rm campusguard make valgrind
docker compose run --rm campusguard make gdb
```

## Build locally

```
make            # builds ./campusguard with -std=c++11 -Wall -Wextra -pedantic -g
make run
make valgrind
make clean
```

## Scenarios (`main.cpp`)

1. **Break-in at Engineering 1.** The operator dispatches the fastest security unit and
   sounds the legacy motion alarm. The guard reports a breach. The coordinator has
   facilities lock the building (all alarms arm through the adapter), puts medical on
   standby and has comms alert the campus. Backup is sent. A student is refused entry
   and a guard is let in, disarming the alarms. Cancelling the lockdown is refused.
   Resolving the incident reopens the building. Dispatching to the resolved incident
   is refused.
2. **Casualty in the Exam Hall.** The operator restricts access, then cancels a second
   restriction (undo restores the first). The dispatch strategy switches to mass
   response and both medical units are sent. The confirmed casualty makes the
   coordinator send a security escort and has facilities hold the lifts. A command
   with a missing alarm fails and is reported.

## Patterns and participants

| Pattern | Role | Classes |
|---|---|---|
| Command | Command / Concrete commands | `OperatorCommand` / `DispatchUnitsCommand`, `SecureBuildingCommand`, `RestrictAccessCommand`, `SoundAlarmCommand`, `ResolveIncidentCommand` |
| | Invoker | `OperatorConsole` (queue, execute, cancel last) |
| | Receivers | `DispatchCentre`, `Building`, `Alarm`, `IncidentMediator` |
| Mediator | Mediator / Concrete mediator | `IncidentMediator` / `IncidentCoordinator` |
| | Colleagues | `ResponseUnit`: `SecurityTeam`, `MedicalTeam`, `FacilitiesTeam`, `CommsTeam` |
| Adapter | Target / Adapter / Adaptee | `Alarm` / `AlarmConnector` / `BlueSecurityAlarm` (legacy integer codes) |
| Facade | Facade | `CampusGuardFacade` (on the `Facade` branch) |
| State | Context / States | `Building` / `UnlockedState`, `LockedState`, `CleaningOnlyState`, `LecturerOnlyState`, `NoStudentsState` |
| Strategy | Context / Strategies | `DispatchCentre` / `FastestUnitStrategy`, `MassResponseStrategy` |

## Ownership policy

| Owner | Owns (deletes) | Holds without owning |
|---|---|---|
| `DispatchCentre` | its `ResponseUnit`s and current `DispatchStrategy` (`setStrategy` deletes the old one) | — |
| `IncidentCoordinator` | nothing | the same units, to route events |
| `Building` | its `Alarm`s and current `BuildingState` | — |
| `LockedState` | the state it replaced, handed back on `reopen()` | — |
| `AlarmConnector` | its `BlueSecurityAlarm` | — |
| `OperatorConsole` | every queued and executed command; failed and cancelled commands are deleted at once | — |
| `RestrictAccessCommand` | the policy it will apply, then the policy it replaced (for undo) | its `Building` |
| `Incident` | nothing | its `Building` |
| `ResponseUnit` | nothing | its current `Incident` and the mediator |

`main` declares objects in lifetime order: buildings, then incidents, then the
coordinator and dispatch centre, then the console. The console is destroyed first, so
no command outlives what it points at. Every polymorphic base (`OperatorCommand`,
`IncidentMediator`, `ResponseUnit`, `Alarm`, `BuildingState`, `DispatchStrategy`) has
a virtual destructor. Classes that own raw pointers have copying disabled.

## Evidence

- Valgrind: [`docs/valgrind.txt`](docs/valgrind.txt). 0 errors, all heap blocks freed.
- GDB investigation of a real bug: [`docs/gdb-investigation.md`](docs/gdb-investigation.md)
