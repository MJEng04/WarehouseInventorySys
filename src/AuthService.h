// AuthService class -> for user authentication.

#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <vector>
#include "Employee.h"
#include "EmployeeRepository.h"

// Handles user authentication and role identification
// AuthService -  validates user IDs -> determines whether user is Manager or Employee.
class AuthService {
private:
    std::string managerId;   // Hardcoded manager user ID
    std::string managerName; // Hardcoded manager name
    EmployeeRepository& employeeRepo; // Refers to employee repo for validation

public:
    // Constructs AuthService with reference to employee repo.
    AuthService(EmployeeRepository& employeeRepo,
                std::string managerId = "BIGBOSS04",
                std::string managerName = "MARTIN");

    // Determines role of user based on ID.
    // Checks manager ID first -> searches active employees and returns "unknown" if ID doesn't match a known user.
    std::string getRole(std::string userId, std::vector<Employee>& employees);


    // Checks if user ID belongs to manager.
    bool isManager(std::string userId);

    // Finds active employee by user ID -> Inactive employees excluded and can't login.
    Employee* findEmployee(std::string userId, std::vector<Employee>& employees);
};

#endif
