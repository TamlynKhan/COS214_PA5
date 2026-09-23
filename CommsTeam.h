#ifndef COMMSTEAM_H
#define COMMSTEAM_H

#include <iostream>
#include "ResponseUnit.h"

class CommsTeam : public ResponseUnit {
private:
    EventType lastEvent;
    std::string lastLocation;
public:
    CommsTeam(IncidentMediator* im);
    ~CommsTeam() override;
    void issueEvacuation(const std::string& location);
    void sendAllClear(const std::string& location);
     EventType get() const override;
    std::string getLocation() const override;
    void set(EventType type, const std::string& location) override;
};

#endif