#ifndef INCIDENTMEDIATOR_H
#define INCIDENTMEDIATOR_H

#include <string>
#include <vector>

class Incident;
class ResponseUnit;

enum class EventType
{
    UNIT_DEPLOYED,
    BREACH,
    CASUALTY,
    BACKUP_REQUESTED
};

std::string eventTypeName(EventType type);

class IncidentMediator
{
    public:
        virtual ~IncidentMediator();

        virtual void notify(ResponseUnit* origin, EventType event, Incident* incident) = 0;
        virtual bool resolveIncident(Incident* incident) = 0;
};

class IncidentCoordinator : public IncidentMediator
{
    public:
        IncidentCoordinator();
        ~IncidentCoordinator() override;

        IncidentCoordinator(const IncidentCoordinator&) = delete;
        IncidentCoordinator& operator=(const IncidentCoordinator&) = delete;

        void addUnit(ResponseUnit* unit);
        void notify(ResponseUnit* origin, EventType event, Incident* incident) override;
        bool resolveIncident(Incident* incident) override;

    private:
        std::vector<ResponseUnit*> selectResponders(ResponseUnit* origin, Incident* incident) const;
        void sendBackup(ResponseUnit* origin, Incident* incident);

        std::vector<ResponseUnit*> units;
};

#endif
