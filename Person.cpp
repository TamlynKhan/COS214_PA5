#include "Person.h"

std::string roleName(Role role)
{
    static const char* const names[] = { "Student", "Tutor", "Lecturer", "Cleaning Staff", "Security" };
    return names[static_cast<int>(role)];
}

Person::Person(const std::string& name, Role role)
    : name(name), role(role)
{
}

std::string Person::getName() const
{
    return name;
}

Role Person::getRole() const
{
    return role;
}

void Person::setName(const std::string& name)
{
    this->name = name;
}

void Person::setRole(Role role)
{
    this->role = role;
}
