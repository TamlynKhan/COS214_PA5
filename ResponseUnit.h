#ifndef RESPONSEUNIT_H
#define RESPONSEUNIT_H

#include <iostream>
#include "IncidentMediator.h"
#include "EventType.h"

class ResponseUnit {
private:
    IncidentMediator* incidentMediator;
public:
    ResponseUnit(IncidentMediator* im) : incidentMediator(im) {}
    virtual ~ResponseUnit() {}
    void changed() { incidentMediator->notify(this); };
    virtual EventType get() const = 0;
    virtual std::string getLocation() const = 0;
    virtual void set(EventType type, const std::string& location) = 0;
};

#endif