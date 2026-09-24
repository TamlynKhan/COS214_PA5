#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <string>

enum class UnitType
{
    SECURITY,
    MEDICAL,
    FACILITIES,
    COMMUNICATIONS
};

std::string unitTypeName(UnitType type);

#endif
