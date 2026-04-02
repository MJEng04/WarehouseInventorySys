#include <iostream>
#include <string>
#include "src/ProductRepository.h"
#include "src/EmployeeRepository.h"
#include "src/OrderRepository.h"
#include "src/AuthService.h"
#include "src/InventoryService.h"
#include "src/EmployeeService.h"
#include "src/OrderService.h"
#include "src/ManagerConsole.h"
#include "SRC/EmployeeConsole.h"

int main() {
    // Initialises data access layer by pointing the repos to CSV files.
    ProductRepository productRepo("data/products.csv");
    EmployeeRepository employeeRepo("data/employees.csv");
    OrderRepository orderRepo("data/orders.csv");

    // Instantiates business logic layer by adding required repo dependencies.
    InventoryService inventoryService(productRepo);
    EmployeeService employeeService(employeeRepo);
    OrderService orderService(orderRepo, productRepo, employeeRepo);
    AuthService authService(employeeRepo);


    // Displays Manager or Employee console depending on role

    std::cout << "----------------------------------------" << std::endl;
    std::cout << " WAREHOUSE INVENTORY MANAGEMENT SYSTEM  " << std::endl;
    std::cout << "----------------------------------------" << std::endl;


    std::string userId;
    while (true) {
        std::cout << "\nEnter User ID (or choose 'exit' to close program): ";
        std::getline(std::cin, userId);

        if (userId == "exit") {
            std::cout << "Exiting Program" << std::endl;
            break;
        }

        std::vector<Employee> employees = employeeRepo.loadAll();
        std::string role = authService.getRole(userId, employees);

        if (role == "MANAGER") {
            ManagerConsole managerConsole(inventoryService, employeeService, orderService);
            managerConsole.showMainMenu();
        } else if (role == "EMPLOYEE") {
            Employee* emp = authService.findEmployee(userId, employees);
            if (emp != nullptr) {
                EmployeeConsole employeeConsole(orderService, inventoryService, *emp);
                employeeConsole.showMainMenu();
            }
        } else {
            std::cout << "Invalid User ID -> Please try again." << std::endl;
        }
    }

    return 0;
}