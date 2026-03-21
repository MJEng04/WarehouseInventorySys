// EmployeeConsole class for employee user interface.
#ifndef EMPLOYEECONSOLE_H
#define EMPLOYEECONSOLE_H

#include "OrderService.h"
#include "InventoryService.h"
#include "Employee.h"


// console interface for employees.
class EmployeeConsole {
private:
    OrderService& orderService;       // for order operations
    InventoryService& inventoryService; // for product search
    Employee currentEmployee;         // Logged-in employee


    // Displays orders assigned to employee with locations.
    void viewAssignedOrders();

    // Allows employee to search for product using ID or name.
    void findProduct();


    // Allows employee to mark assigned order as complete.
    void completeOrder();


    // Displays employee report and exports CSV files.
    void handleReports();

    void printDivider();                  // Prints separator line
    void waitForEnter();                  // Pauses until the user presses Enter
    int getMenuChoice(int min, int max);  // Reads and validates menu selection

public:
    // Constructs EmployeeConsole for user.
    EmployeeConsole(OrderService& orderService,
                    InventoryService& inventoryService,
                    Employee currentEmployee);

    // Displays main employee menu.
    void showMainMenu();
};

#endif
