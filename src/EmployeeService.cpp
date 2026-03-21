// EmployeeService class.
#include "EmployeeService.h"
#include <stdexcept>

// Constructs EmployeeService with repo reference.
EmployeeService::EmployeeService(EmployeeRepository& employeeRepo)
    : employeeRepo(employeeRepo) {}

// Performs linear search to check for duplicate employee IDs.
bool EmployeeService::idExists(std::string employeeId, std::vector<Employee>& employees) {
    for (Employee& e : employees) {
        if (e.getUserId() == employeeId) return true;
    }
    return false;
}


// Adds new employee after validating ID.
void EmployeeService::addEmployee(Employee employee) {
    std::vector<Employee> employees = employeeRepo.loadAll();

    if (idExists(employee.getUserId(), employees)) {
        throw std::runtime_error("Employee ID already exists: " + employee.getUserId());
    }

    employees.push_back(employee);
    employeeRepo.saveAll(employees);
}


// Updates employee's name by finding them in list -> modifies -> saves list.
void EmployeeService::editEmployee(std::string employeeId, std::string newName) {
    std::vector<Employee> employees = employeeRepo.loadAll();
    Employee* e = employeeRepo.findById(employees, employeeId);

    if (e == nullptr) {
        throw std::runtime_error("Employee not found: " + employeeId);
    }

    e->setName(newName);
    employeeRepo.saveAll(employees);
}

// Returns employees from file -> including inactive ones.
std::vector<Employee> EmployeeService::getAllEmployees() {
    return employeeRepo.loadAll();
}


// Filters employee list -> returns active employees.
std::vector<Employee> EmployeeService::getActiveEmployees() {
    std::vector<Employee> all = employeeRepo.loadAll();
    std::vector<Employee> active;
    for (Employee& e : all) {
        if (e.isActiveUser()) active.push_back(e);
    }
    return active;
}


// Deactivates employee after checking if they have any outstanding orders.
void EmployeeService::deactivateEmployee(std::string employeeId, std::vector<Order>& orders) {
    for (Order& o : orders) {
        if (o.getAssignedEmployeeId() == employeeId &&
            o.getStatus() != OrderStatus::COMPLETED) {
            throw std::runtime_error(
                "Can't deactivate employee " + employeeId +
                " - they have pending or assigned orders.");
        }
    }

    std::vector<Employee> employees = employeeRepo.loadAll();
    Employee* e = employeeRepo.findById(employees, employeeId);

    if (e == nullptr) {
        throw std::runtime_error("Employee not found: " + employeeId);
    }

    e->setActive(false); // Soft delete - data still preserved in file
    employeeRepo.saveAll(employees);
}


// Reactivates deactivated employee.
void EmployeeService::reactivateEmployee(std::string employeeId) {
    std::vector<Employee> employees = employeeRepo.loadAll();
    Employee* e = employeeRepo.findById(employees, employeeId);

    if (e == nullptr) {
        throw std::runtime_error("Employee not found: " + employeeId);
    }

    e->setActive(true);
    employeeRepo.saveAll(employees);
}


// Finds employee by ID.
Employee* EmployeeService::findById(std::string employeeId) {
    std::vector<Employee> employees = employeeRepo.loadAll();
    return employeeRepo.findById(employees, employeeId);
}
