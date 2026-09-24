#ifndef DISPATCHSTRATEGY_H
#define DISPATCHSTRATEGY_H

#include <string>
#include <vector>

class Incident;
class ResponseUnit;

class DispatchStrategy
{
    public:
        virtual ~DispatchStrategy();

        virtual std::vector<ResponseUnit*> selectUnits(const Incident& incident, const std::vector<ResponseUnit*>& roster) const = 0;
        virtual std::string getName() const = 0;
};

class FastestUnitStrategy : public DispatchStrategy
{
    public:
        std::vector<ResponseUnit*> selectUnits(const Incident& incident, const std::vector<ResponseUnit*>& roster) const override;
        std::string getName() const override;
};

class MassResponseStrategy : public DispatchStrategy
{
    public:
        std::vector<ResponseUnit*> selectUnits(const Incident& incident, const std::vector<ResponseUnit*>& roster) const override;
        std::string getName() const override;
};

#endif
