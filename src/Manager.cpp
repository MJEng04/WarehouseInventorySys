#include "Manager.h"

// Assigns construction to parent User class using initialiser list.
Manager::Manager(std::string userId, std::string name, bool isActive)
    : User(userId, name, isActive) {}

// Implementation of role function for manager users.
std::string Manager::getRole() const {
    return "MANAGER";
}