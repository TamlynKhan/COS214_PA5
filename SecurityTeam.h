#ifndef SECURITYTEAM_H
#define SECURITYTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class SecurityTeam : public ResponseUnit {
private:
    std::string lastEvent;
public:
    SecurityTeam(IncidentMediator* im);
    ~SecurityTeam() override;
   
    std::string get() override;
    void set(std::string change) override;
};

#endif