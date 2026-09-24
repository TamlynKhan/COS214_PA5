#ifndef ALARM_H
#define ALARM_H

#include <string>

class BlueSecurityAlarm;

enum class AlarmType
{
    FIRE,
    WINDOW,
    MOTION
};

std::string alarmTypeName(AlarmType type);

class Alarm
{
    public:
        Alarm(const std::string& name, AlarmType type);
        virtual ~Alarm();

        std::string getName() const;
        AlarmType getType() const;

        virtual bool activate() = 0;
        virtual bool deactivate() = 0;
        virtual bool isActive() const = 0;

    private:
        std::string name;
        AlarmType type;
};

class AlarmConnector : public Alarm
{
    public:
        AlarmConnector(const std::string& name, AlarmType type);
        ~AlarmConnector() override;

        AlarmConnector(const AlarmConnector&) = delete;
        AlarmConnector& operator=(const AlarmConnector&) = delete;

        bool activate() override;
        bool deactivate() override;
        bool isActive() const override;

    private:
        int translateCode(AlarmType type) const;

        BlueSecurityAlarm* legacyAlarm;
};

#endif
