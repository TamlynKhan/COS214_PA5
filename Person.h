#ifndef PERSON_H
#define PERSON_H

#include <string>

enum class Role
{
    STUDENT,
    TUTOR,
    LECTURER,
    CLEANING_STAFF,
    SECURITY
};

std::string roleName(Role role);

class Person
{
    public:
        Person(const std::string& name, Role role);

        std::string getName() const;
        Role getRole() const;

        void setName(const std::string& name);
        void setRole(Role role);

    private:
        std::string name;
        Role role;
};

#endif
