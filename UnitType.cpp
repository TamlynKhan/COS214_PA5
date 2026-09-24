#include "UnitType.h"

std::string unitTypeName(UnitType type)
{
    static const char* const names[] = { "Security", "Medical", "Facilities", "Communications" };
    return names[static_cast<int>(type)];
}
