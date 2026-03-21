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

    void handleProducts();    // for product-related tasks.
    void addProduct();        // for adding new product.
    void viewProducts();      // for listing products.
    void editProduct();       // for updating product details.
    void deactivateProduct(); // for soft-deleting product.
    
    void handleOrders();  // for order-related tasks.
    void createOrder();   // for starting a order.
    void viewOrders();    // for displaying orders.
    void editOrder();     // for updating order items.
    void assignOrder();   // for assigning orders to employees.

    void handleReports(); // for generating CSV exports.

    void printDivider();                
    void waitForEnter();                
    int getMenuChoice(int min, int max);

public:
    // Links console UI to backend.
    ManagerConsole(InventoryService& inventoryService,
                   EmployeeService& employeeService,
                   OrderService& orderService);

    void showMainMenu();
};

#endif