#ifndef RESPPONSEUNIT_H
#define RESPPONSEUNIT_H

class IncidentMediator;
#include <iostream>

class ResponseUnit {
private:
    IncidentMediator* incidentMediator;
public:
    ResponseUnit(IncidentMediator* im) : incidentMediator(im) {}
    virtual ~ResponseUnit() {}
    void change() { incidentMediator->notify(this); };
    virtual std::string get() = 0;
    virtual void set(std::string change) = 0;
};

#endif