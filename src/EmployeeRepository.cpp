// EmployeeRepository class.
#include "EmployeeRepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>


// Constructs EmployeeRepository with file path.
EmployeeRepository::EmployeeRepository(std::string filePath)
    : filePath(filePath) {}

// Loads employees from CSV -> including assigned order ID lists.
std::vector<Employee> EmployeeRepository::loadAll() {
    std::vector<Employee> employees;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return employees; // Returns empty list on initial run
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string userId, name, isActiveStr, orderIdsStr;

        std::getline(ss, userId, ',');
        std::getline(ss, name, ',');
        std::getline(ss, isActiveStr, ',');
        std::getline(ss, orderIdsStr); // Remainder of line -> order IDs

        try {
            bool isActive = (isActiveStr == "1");
            Employee emp(userId, name, isActive);

            // Analyse order IDs and add each to employee
            if (!orderIdsStr.empty()) {
                std::stringstream orderStream(orderIdsStr);
                std::string orderId;
                while (std::getline(orderStream, orderId, '|')) {
                    if (!orderId.empty()) {
                        emp.addOrderId(orderId);
                    }
                }
            }

            employees.push_back(emp);
        } catch (...) {
            continue; // Skip malformed lines
        }
    }

    file.close();
    return employees;
}


// Saves all employees to CSV -> including assigned order ID lists.
void EmployeeRepository::saveAll(std::vector<Employee> employees) {
    std::ofstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filePath);
    }

    for (Employee& e : employees) {
        file << e.getUserId() << ","
             << e.getName() << ","
             << (e.isActiveUser() ? "1" : "0") << ",";

        // Write order IDs as pipe-separated values
        std::vector<std::string> orderIds = e.getAssignedOrderIds();
        for (int i = 0; i < orderIds.size(); i++) {
            file << orderIds[i];
            if (i < orderIds.size() - 1) file << "|";
        }

        file << "\n";
    }

    file.close();
}


// Linear search for an employee by user ID. O(n) time complexity.
Employee* EmployeeRepository::findById(std::vector<Employee>& employees, std::string id) {
    for (Employee& e : employees) {
        if (e.getUserId() == id) {
            return &e;
        }
    }
    return nullptr;
}
