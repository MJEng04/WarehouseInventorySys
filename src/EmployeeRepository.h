// Defines EmployeeRepository class for data persistence.

#ifndef EMPLOYEEREPOSITORY_H
#define EMPLOYEEREPOSITORY_H

#include <vector>
#include <string>
#include "Employee.h"


// Handles file I/O operations for Employee data.
class EmployeeRepository {
private:
    std::string filePath; // Path to employees CSV file

public:
    // Constructs EmployeeRepository dirrecting to given file.
    EmployeeRepository(std::string filePath);

    // Loads employees from CSV file.
    std::vector<Employee> loadAll();

    // Saves employee list to CSV file.
    void saveAll(std::vector<Employee> employees);

    // Finds employee by user ID within loaded vector.
    Employee* findById(std::vector<Employee>& employees, std::string id);
};

#endif
