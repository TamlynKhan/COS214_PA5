#ifndef DISPATCHCENTRE_H
#define DISPATCHCENTRE_H

#include <vector>

class DispatchStrategy;
class Incident;
class ResponseUnit;

class DispatchCentre
{
    public:
        explicit DispatchCentre(DispatchStrategy* strategy);
        ~DispatchCentre();

        DispatchCentre(const DispatchCentre&) = delete;
        DispatchCentre& operator=(const DispatchCentre&) = delete;

        void addUnit(ResponseUnit* unit);
        void setStrategy(DispatchStrategy* newStrategy);

        std::vector<ResponseUnit*> dispatch(Incident* incident);
        int recall(Incident* incident, const std::vector<ResponseUnit*>& units);
        void printRoster() const;

    private:
        std::vector<ResponseUnit*> roster;
        DispatchStrategy* strategy;
};

#endif
