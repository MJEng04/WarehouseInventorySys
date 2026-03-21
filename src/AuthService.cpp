#include "AuthService.h"

AuthService::AuthService(EmployeeRepository& employeeRepo,std::string managerId,std::string managerName)
: employeeRepo(employeeRepo),managerId(managerId),managerName(managerName) {}

bool AuthService::isManager(std::string userId) {
    return userId == managerId;
}

// Returns active employees - inactive ones can no longer log in -> NFR-S2
Employee* AuthService::findEmployee(std::string userId, std::vector<Employee>& employees) {
    for (Employee& e : employees) {
        if (e.getUserId() == userId && e.isActiveUser()) {
            return &e;
        }
    }
    return nullptr;
}

// Checks Roles
std::string AuthService::getRole(std::string userId, std::vector<Employee>& employees) {
    if (isManager(userId)) {
        return "MANAGER";
    }
    if (findEmployee(userId, employees) != nullptr) {
        return "EMPLOYEE";
    }
    return "UNKNOWN";
}
