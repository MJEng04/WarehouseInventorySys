// EmployeeService class for employee management business logic.
#ifndef EMPLOYEESERVICE_H
#define EMPLOYEESERVICE_H

#include <vector>
#include <string>
#include "Employee.h"
#include "Order.h"
#include "EmployeeRepository.h"

// Handles business logic for employee management.
class EmployeeService {
private:
    EmployeeRepository& employeeRepo; // Refers to employee data repo

    // Checks if employee ID already exists.
    bool idExists(std::string employeeId, std::vector<Employee>& employees);

public:
    // Constructs EmployeeService with repo reference. 
    EmployeeService(EmployeeRepository& employeeRepo);

    // Adds new employee to system -> Validates employee ID before it saves.
    void addEmployee(Employee employee);

    // Updates employee's name.
    void editEmployee(std::string employeeId, std::string newName);


    // Returns employees -> including inactive ones.
    std::vector<Employee> getAllEmployees();

    // Returns active employees.
    std::vector<Employee> getActiveEmployees();

    // Deactivates employee -> prevents new orders being assigned.
    void deactivateEmployee(std::string employeeId, std::vector<Order>& orders);

    // Reactivates deactivated employee.
    void reactivateEmployee(std::string employeeId);

    // Finds employee by ID.
    Employee* findById(std::string employeeId);
};

#endif
