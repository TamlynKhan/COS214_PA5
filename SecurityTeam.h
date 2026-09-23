#ifndef SECURITYTEAM_H
#define SECURITYTEAM_H

#include <iostream>
#include "ResponseUnit.h"
#include "EventType.h"

class SecurityTeam : public ResponseUnit {
private:
    EventType lastEvent;
    std::string lastLocation;
public:
    SecurityTeam(IncidentMediator* im);
    ~SecurityTeam() override;
    void reportBreach(const std::string& location);
    void requestBackup();
    EventType get() const override;
    std::string getLocation() const override;
    void set(EventType type, const std::string& location) override;
};

#endif