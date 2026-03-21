#ifndef MANAGER_H
#define MANAGER_H

#include "User.h"

// Represents manager user with admin system access.
class Manager : public User {
public:
    // Constructs manager and sends basic info to User base class.
    Manager(std::string userId, std::string name, bool isActive = true);

    // Identifies user as Manager.
    std::string getRole() const override;
};

#endif