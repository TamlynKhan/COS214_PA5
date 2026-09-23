#ifndef FACILITIESTEAM_H
#define FACILITIESTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class FacilitiesTeam : public ResponseUnit {
private:
    std::string lastEvent;
public:
    FacilitiesTeam(IncidentMediator* im);
    ~FacilitiesTeam() override;
    
    std::string get() override;
    void set(std::string change) override;
};

#endif