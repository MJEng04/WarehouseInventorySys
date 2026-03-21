// Implementation of Employee class
#include "Employee.h"
#include <algorithm>

// Constructs Employee using User's base constructor
Employee::Employee(std::string userId, std::string name, bool isActive)
    : User(userId, name, isActive) {}


// Returns "employee" - used by AuthService to route user to the EmployeeConsole.
std::string Employee::getRole() const {
    return "EMPLOYEE";
}

std::vector<std::string> Employee::getAssignedOrderIds() const {
    return assignedOrderIds;
}


// Appends order ID to end of assigned orders list.
void Employee::addOrderId(std::string orderId) {
    assignedOrderIds.push_back(orderId);
}

// Removes specific order ID from assigned orders list.
void Employee::removeOrderId(std::string orderId) {
    assignedOrderIds.erase(
        std::remove(assignedOrderIds.begin(), assignedOrderIds.end(), orderId),
        assignedOrderIds.end()
    );
}
