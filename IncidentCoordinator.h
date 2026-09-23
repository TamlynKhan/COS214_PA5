#ifndef INCIDENTCOORDINATOR_H
#define INCIDENTCOORDINATOR_H

class ResponseUnit;
#include <iostream>
#include <vector>
#include "IncidentMediator.h"

class IncidentCoordinator : public IncidentMediator {
protected:
    std::vector<ResponseUnit*> units;
public:
    IncidentCoordinator();
    ~IncidentCoordinator() override;
    void addUnit(ResponseUnit* unit);
    virtual void notify(ResponseUnit* origin) override;
    void resolveIncident(const std::string& location);
};

#endif