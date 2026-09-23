//state

#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>

using namespace std;

enum Role {
    STUDENT,
    TUTOR,
    LECTURER,
    CLEANINGSTAFF,
    SECURITY
};

class Person {
    private:
        string name;
        Role role;

    public:
        Person(string name, Role role) : name(name), role(role) {}

        Role getRole() {
            return role;
        }
        string getName() {
            return name;
        }

        void setRole(Role role) {
            this->role = role;
        }
        void setName(string name) {
            this->name = name;
        }
};

#endif