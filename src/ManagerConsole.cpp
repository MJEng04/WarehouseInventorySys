#include "ManagerConsole.h"
#include <iostream>
#include <string>
#include <limits>

// Adds service layer into console UI.
ManagerConsole::ManagerConsole(InventoryService& inventoryService,
                               EmployeeService& employeeService,
                               OrderService& orderService)
    : inventoryService(inventoryService),
      employeeService(employeeService),
      orderService(orderService) {}


void ManagerConsole::printDivider() {
    std::cout << "----------------------------------------" << std::endl;
}

void ManagerConsole::waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}