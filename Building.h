#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <vector>

class Alarm;
class BuildingState;
class Person;

class Building
{
    public:
        explicit Building(const std::string& name, BuildingState* initialState = nullptr);
        ~Building();

        Building(const Building&) = delete;
        Building& operator=(const Building&) = delete;

        std::string getName() const;
        std::string getStateName() const;

        void addAlarm(Alarm* alarm);
        Alarm* findAlarm(const std::string& alarmName) const;
        void activateAlarms();
        void deactivateAlarms();

        bool access(const Person& person);
        bool lockdown();
        bool reopen();

    private:
        void changeState(BuildingState* next);

        std::string name;
        BuildingState* state;
        std::vector<Alarm*> alarms;
};

#endif
