#ifndef MANAGERCONSOLE_H
#define MANAGERCONSOLE_H

#include "InventoryService.h"
#include "EmployeeService.h"
#include "OrderService.h"

// Manages user interface for admin tasks.
class ManagerConsole {
private:
    InventoryService& inventoryService; // for managing products.
    EmployeeService& employeeService;   // for managing system users.
    OrderService& orderService;         // for managing order lifecycle.

    // Product Section
    void handleProducts();     // for product-related tasks.
    void addProduct();         // for adding new product.
    void viewProducts();       // for listing products.
    void editProduct();        // for updating product details.
    void deactivateProduct();  // for soft-deleting product.
    void reactivateProduct();  // for reactivating inactive products

    // Employee Section
    void handleEmployees();    // for employee related tasks
    void addEmployee();        // for adding new employees
    void viewEmployees();      // for listing all employees
    void editEmployee();       // for updating employee details
    void deactivateEmployee(); // for soft-deleting employees
    void reactivateEmployee(); // for reactivating inactive employees
    
    // Order Section
    void handleOrders();      // for order-related tasks.
    void createOrder();       // for starting a order.
    void viewOrders();        // for displaying orders.
    void editOrder();         // for updating order items.
    void assignOrder();       // for assigning orders to employees.

    // Reports
    void handleReports();     // for generating and exporting CSV data reports.

    // Misc Methods
    void printDivider();                 // for displaying visual separators in console.          
    void waitForEnter();                 // for pausing until user responds.
    int getMenuChoice(int min, int max); // for validating user input within range.

public:
    // Links console UI to backend.
    ManagerConsole(InventoryService& inventoryService,
                   EmployeeService& employeeService,
                   OrderService& orderService);

    void showMainMenu();
};

#endif

