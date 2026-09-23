#ifndef MEDICALTEAM_H
#define MEDICALTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class MedicalTeam : public ResponseUnit {
private:
    std::string lastEvent;
public:
    MedicalTeam(IncidentMediator* im);
    ~MedicalTeam() override;
   
    std::string get() override;
    void set(std::string change) override;
};

#endif