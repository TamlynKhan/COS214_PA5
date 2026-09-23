#ifndef COMMSTEAM_H
#define COMMSTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class CommsTeam : public ResponseUnit {
private:
    std::string lastEvent;
public:
    CommsTeam(IncidentMediator* im);
    ~CommsTeam() override;
    void issueEvacuation(const std::string& location);
    void sendClear(const std::string& location);
    std::string get() override;
    void set(std::string change) override;
};

#endif