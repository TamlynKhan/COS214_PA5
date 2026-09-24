#include "DispatchStrategy.h"
#include "Incident.h"
#include "ResponseUnit.h"

DispatchStrategy::~DispatchStrategy()
{
}

std::vector<ResponseUnit*> FastestUnitStrategy::selectUnits(const Incident& incident, const std::vector<ResponseUnit*>& roster) const
{
    ResponseUnit* fastest = nullptr;
    for (ResponseUnit* unit : roster)
    {
        if (unit->canRespondTo(incident) && (fastest == nullptr || unit->getResponseMinutes() < fastest->getResponseMinutes()))
        {
            fastest = unit;
        }
    }

    std::vector<ResponseUnit*> selected;
    if (fastest != nullptr)
    {
        selected.push_back(fastest);
    }
    return selected;
}

std::string FastestUnitStrategy::getName() const
{
    return "Fastest Unit";
}

std::vector<ResponseUnit*> MassResponseStrategy::selectUnits(const Incident& incident, const std::vector<ResponseUnit*>& roster) const
{
    std::vector<ResponseUnit*> selected;
    for (ResponseUnit* unit : roster)
    {
        if (unit->canRespondTo(incident))
        {
            selected.push_back(unit);
        }
    }
    return selected;
}

std::string MassResponseStrategy::getName() const
{
    return "Mass Response";
}
