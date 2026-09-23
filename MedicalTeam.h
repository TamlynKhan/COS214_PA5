#ifndef MEDICALTEAM_H
#define MEDICALTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class MedicalTeam : public ResponseUnit {
private:
    EventType lastEvent;
    std::string lastLocation;
public:
    MedicalTeam(IncidentMediator* im);
    ~MedicalTeam() override;
    void confrimCasualty(const std::string& location);
    void standBy();
    EventType get() const override;
    std::string getLocation() const override;
    void set(EventType type, const std::string& location) override;
};

#endif