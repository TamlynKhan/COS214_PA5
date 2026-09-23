#ifndef INCIDENTMEDIATOR_H
#define INCIDENTMEDIATOR_H

class ResponseUnit;
#include <iostream>

class IncidentMediator {
public:
    virtual ~IncidentMediator() {}
    virtual void notify(ResponseUnit* responseUnit) = 0;
};

#endif