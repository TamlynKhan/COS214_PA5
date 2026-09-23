#ifndef FACILITIESTEAM_H
#define FACILITIESTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class FacilitiesTeam : public ResponseUnit {
private:
    EventType lastEvent;
    std::string lastLocation;
public:
    FacilitiesTeam(IncidentMediator* im);
    ~FacilitiesTeam() override;
    void lockArea(const std::string& location);
    void openEvacuationRoute(const std::string& location);
    EventType get() const override;
    std::string getLocation() const override;
    void set(EventType type, const std::string& location) override;
};

#endif