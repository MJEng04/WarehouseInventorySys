#include "User.h"

// Assigns core user attributes.
User::User(std::string userId, std::string name, bool isActive)
    : userId(userId), name(name), isActive(isActive) {}

std::string User::getUserId() const { return userId; }
std::string User::getName() const { return name; }
bool User::isActiveUser() const { return isActive; }

void User::setName(std::string newName) { name = newName; }
void User::setActive(bool active) { isActive = active; }