#ifndef RESPONSEUNIT_H
#define RESPONSEUNIT_H

#include <string>

#include "IncidentMediator.h"
#include "UnitType.h"

class Incident;

class ResponseUnit
{
    public:
        ResponseUnit(const std::string& callSign, UnitType specialty, int responseMinutes);
        virtual ~ResponseUnit();

        ResponseUnit(const ResponseUnit&) = delete;
        ResponseUnit& operator=(const ResponseUnit&) = delete;

        void setMediator(IncidentMediator* incidentMediator);

        std::string getCallSign() const;
        UnitType getSpecialty() const;
        int getResponseMinutes() const;
        Incident* getAssignment() const;
        bool isAvailable() const;
        bool canRespondTo(const Incident& incident) const;

        void deploy(Incident* incident);
        void standDown();

        virtual void onResponderDeployed(ResponseUnit* responder, Incident* incident);
        virtual void onBreach(Incident* incident);
        virtual void onCasualty(Incident* incident);
        virtual void onIncidentResolved(Incident* incident);

    protected:
        void changed(EventType event, Incident* incident);

    private:
        std::string callSign;
        UnitType specialty;
        int responseMinutes;
        Incident* assignment;
        IncidentMediator* incidentMediator;
};

class SecurityTeam : public ResponseUnit
{
    public:
        SecurityTeam(const std::string& callSign, int responseMinutes);

        bool reportBreach();
        bool requestBackup();

        void onCasualty(Incident* incident) override;
};

class MedicalTeam : public ResponseUnit
{
    public:
        MedicalTeam(const std::string& callSign, int responseMinutes);

        bool confirmCasualty();

        void onBreach(Incident* incident) override;
};

class FacilitiesTeam : public ResponseUnit
{
    public:
        FacilitiesTeam(const std::string& callSign, int responseMinutes);

        void onBreach(Incident* incident) override;
        void onCasualty(Incident* incident) override;
        void onIncidentResolved(Incident* incident) override;
};

class CommsTeam : public ResponseUnit
{
    public:
        CommsTeam(const std::string& callSign, int responseMinutes);

        void onResponderDeployed(ResponseUnit* responder, Incident* incident) override;
        void onBreach(Incident* incident) override;
        void onCasualty(Incident* incident) override;
        void onIncidentResolved(Incident* incident) override;
};

#endif
