#include <iostream>
#include "src/ProductRepository.h"
#include "src/EmployeeRepository.h"
#include "src/OrderRepository.h"
#include "src/AuthService.h"
#include "src/InventoryService.h"
#include "src/EmployeeService.h"
#include "src/OrderService.h"
#include "src/ManagerConsole.h"

int main() {
    // Initialises data access layer by pointing the repos to CSV files.
    ProductRepository productRepo("data/products.csv");
    EmployeeRepository employeeRepo("data/employees.csv");
    OrderRepository orderRepo("data/orders.csv");

    // Instantiates business logic layer by adding required repo dependencies.
    InventoryService inventoryService(productRepo);
    EmployeeService employeeService(employeeRepo);
    OrderService orderService(orderRepo, productRepo, employeeRepo);

    // Launches admin user interface.
    ManagerConsole managerConsole(inventoryService, employeeService, orderService);
    managerConsole.showMainMenu();

    return 0;
}