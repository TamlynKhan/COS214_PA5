# GDB investigation: building stays locked after its incident is resolved

## Symptom

A security incident at the Lab was resolved, but the Lab stayed `Locked`. Nothing
in the output explained why: no error, and no `restoring normal access` line from
the facilities team.

Reproduction (the facilities unit is busy at a second incident in another building):

```cpp
Incident leak("INC-1", "Burst pipe", &library, UnitType::FACILITIES);
Incident theft("INC-2", "Break-in", &lab, UnitType::SECURITY);
centre.dispatch(&leak);                // FAC-1 -> INC-1 (Library)
centre.dispatch(&theft);               // SEC-1 -> INC-2 (Lab)
lab.lockdown();
coordinator.resolveIncident(&theft);
std::cout << "Lab after resolve: " << lab.getStateName() << std::endl;
```

Output: `Lab after resolve: Locked`

## Session

`FacilitiesTeam::onIncidentResolved` is the only code that reopens a building after an
incident, so the first question was whether it runs at all. A second breakpoint stops
`resolveIncident` just before it notifies the responders it selected.

```
$ g++ -std=c++11 -g -O0 -o repro *.cpp
$ gdb -q ./repro
(gdb) break FacilitiesTeam::onIncidentResolved
Breakpoint 1 at 0xfce5: file ResponseUnit.cpp, line 181.
(gdb) break IncidentMediator.cpp:99
Breakpoint 2 at 0x9bdc: file IncidentMediator.cpp, line 99.
(gdb) run
[Building Lab] Unlocked -> Locked
[IncidentCoordinator] resolving INC-2 at Lab

Breakpoint 2, IncidentCoordinator::resolveIncident (this=0x7fffffffd130, incident=0x7fffffffd2a0) at IncidentMediator.cpp:99
99	    for (ResponseUnit* unit : responders)
(gdb) print incident->id
$1 = "INC-2"
(gdb) print responders.size()
$2 = 2
(gdb) print responders[0]->callSign
$3 = "SEC-1"
(gdb) print units.size()
$4 = 3
(gdb) print units[1]->callSign
$5 = "FAC-1"
(gdb) print units[1]->assignment->id
$6 = "INC-1"
(gdb) print units[1]->isAvailable()
$7 = false
(gdb) print units[1]->assignment == incident
$8 = false
(gdb) continue
Lab after resolve: Locked
[Inferior 1 (process 6085) exited normally]
```

Breakpoint 1 was never hit. Three units are registered, but only two were selected to
hear about the resolution, and FAC-1 was not one of them.

## Root cause

`IncidentCoordinator::selectResponders` only picked a unit if it was
`isAvailable() || getAssignment() == incident`. FAC-1 was assigned to INC-1 in the
Library, so both values printed above (`$7`, `$8`) were `false`. The mediator never
told the facilities team, so nothing reopened the Lab. A breach at the Lab failed the
same way: no lockdown, and no message saying so.

## Fix

- `selectResponders` now picks one responder per specialty, preferring a unit on this
  incident, then an available unit, then a unit busy elsewhere
  ([IncidentMediator.cpp](../IncidentMediator.cpp)).
- The team handlers decide what a busy unit can still do. Facilities and comms act
  remotely. Security and medical say they are tied up instead of acting at two sites
  ([ResponseUnit.cpp](../ResponseUnit.cpp)).
- `FacilitiesTeam::onIncidentResolved` asks the mediator (`openIncidentAt`) whether
  another incident is still open in that building before reopening it.

## Verification

The same reproduction after the fix:

```
[IncidentCoordinator] resolving INC-2 at Lab
[SEC-1] standing down from INC-2
[Incident INC-2] status Dispatched -> Resolved
[FAC-1] restoring normal access to Lab
[Building Lab] Locked -> Unlocked
[COM-1] sending an all-clear for Lab
Lab after resolve: Unlocked
```

To run GDB inside Docker:

```
docker compose run --rm campusguard make gdb
```
