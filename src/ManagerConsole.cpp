#include "ManagerConsole.h"
#include <iostream>
#include <string>
#include <limits>

ManagerConsole::ManagerConsole(InventoryService& inventoryService,
                               EmployeeService& employeeService,
                               OrderService& orderService)
    : inventoryService(inventoryService),
      employeeService(employeeService),
      orderService(orderService) {}

//----------------
// UTILITY METHODS
//----------------

void ManagerConsole::printDivider() {
    std::cout << "----------------------------------------" << std::endl;
}

void ManagerConsole::waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Input validation
int ManagerConsole::getMenuChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "Enter choice: ";
        if (std::cin >> choice && choice >= min && choice <= max) {
            std::cin.ignore(); 
            return choice;
        }
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number between "
                  << min << " and " << max << "." << std::endl;
    }
}

//----------
// MAIN MENU
//----------

void ManagerConsole::showMainMenu() {
    int choice;
    do {
        printDivider();
        std::cout << "   MANAGER MENU - Welcome, Manager" << std::endl;
        printDivider();
        std::cout << "1. Manage Products" << std::endl;
        std::cout << "2. Manage Employees" << std::endl;
        std::cout << "3. Manage Orders" << std::endl;
        std::cout << "4. Generate Reports" << std::endl;
        std::cout << "0. Logout" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 4);

        switch (choice) {
            case 1: handleProducts(); break;
            case 2: handleEmployees(); break;
            case 3: handleOrders(); break;
            case 4: handleReports(); break;
            case 0: std::cout << "Logging out..." << std::endl; break;
        }
    } while (choice != 0);
}

//-------------
// PRODUCT MENU
//-------------

void ManagerConsole::handleProducts() {
    int choice;
    do {
        printDivider();
        std::cout << "   PRODUCT MANAGEMENT" << std::endl;
        printDivider();
        std::cout << "1. Add Product" << std::endl;
        std::cout << "2. View All Products" << std::endl;
        std::cout << "3. Edit Product" << std::endl;
        std::cout << "4. Deactivate Product" << std::endl;
        std::cout << "5. Reactivate Product" << std::endl;
        std::cout << "0. Back" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 5);

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: editProduct(); break;
            case 4: deactivateProduct(); break;
            case 5: reactivateProduct(); break;
        }
    } while (choice != 0);
}

void ManagerConsole::addProduct() {
    printDivider();
    std::cout << "   ADD PRODUCT" << std::endl;
    printDivider();

    std::string id, name, location;
    int quantity;
    double price;

    std::cout << "Product ID: ";
    std::getline(std::cin, id);
    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Quantity: ";
    std::cin >> quantity;
    std::cin.ignore();
    std::cout << "Price: ";
    std::cin >> price;
    std::cin.ignore();
    std::cout << "Location (e.g. Aisle A Shelf 1): ";
    std::getline(std::cin, location);

    try {
        inventoryService.addProduct(Product(id, name, quantity, price, location));
        std::cout << "\nProduct added successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::viewProducts() {
    printDivider();
    std::cout << "   ALL PRODUCTS" << std::endl;
    printDivider();

    std::vector<Product> products = inventoryService.getAllProducts();

    if (products.empty()) {
        std::cout << "No products found." << std::endl;
    } else {
        std::cout << "ID\t\t| Name\t\t\t| Qty\t| Price\t| Location\t\t| Status" << std::endl;
        printDivider();
        for (Product& p : products) {
            std::cout << p.getProductId() << "\t| "
                      << p.getName() << "\t| "
                      << p.getQuantity() << "\t| "
                      << p.getPrice() << "\t| "
                      << p.getLocation() << "\t| "
                      << (p.isActiveProduct() ? "Active" : "Inactive")
                      << std::endl;
        }
    }
    waitForEnter();
}

void ManagerConsole::editProduct() {
    printDivider();
    std::cout << "   EDIT PRODUCT" << std::endl;
    printDivider();

    std::string id, name, location;
    int quantity;
    double price;

    std::cout << "Enter Product ID to edit: ";
    std::getline(std::cin, id);
    std::cout << "New Name: ";
    std::getline(std::cin, name);
    std::cout << "New Quantity: ";
    std::cin >> quantity;
    std::cin.ignore();
    std::cout << "New Price: ";
    std::cin >> price;
    std::cin.ignore();
    std::cout << "New Location: ";
    std::getline(std::cin, location);

    try {
        inventoryService.editProduct(id, name, quantity, price, location);
        std::cout << "\nProduct updated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::deactivateProduct() {
    printDivider();
    std::cout << "   DEACTIVATE PRODUCT" << std::endl;
    printDivider();

    std::string id;
    std::cout << "Enter Product ID to deactivate: ";
    std::getline(std::cin, id);

    try {
        // Pass current orders so service can check for pending order conflicts
        std::vector<Order> orders = orderService.getAllOrders();
        inventoryService.deactivateProduct(id, orders);
        std::cout << "\nProduct deactivated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::reactivateProduct() {
    printDivider();
    std::cout << "   REACTIVATE PRODUCT" << std::endl;
    printDivider();

    std::string id;
    std::cout << "Enter Product ID to reactivate: ";
    std::getline(std::cin, id);

    try {
        inventoryService.reactivateProduct(id);
        std::cout << "\nProduct reactivated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

//---------
// EMPLOYEE
//---------

void ManagerConsole::handleEmployees() {
    int choice;
    do {
        printDivider();
        std::cout << "   EMPLOYEE MANAGEMENT" << std::endl;
        printDivider();
        std::cout << "1. Add Employee" << std::endl;
        std::cout << "2. View All Employees" << std::endl;
        std::cout << "3. Edit Employee" << std::endl;
        std::cout << "4. Deactivate Employee" << std::endl;
        std::cout << "5. Reactivate Employee" << std::endl;
        std::cout << "0. Back" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 5);

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: viewEmployees(); break;
            case 3: editEmployee(); break;
            case 4: deactivateEmployee(); break;
            case 5: reactivateEmployee(); break;
        }
    } while (choice != 0);
}

void ManagerConsole::addEmployee() {
    printDivider();
    std::cout << "   ADD EMPLOYEE" << std::endl;
    printDivider();

    std::string id, name;
    std::cout << "Employee ID: ";
    std::getline(std::cin, id);
    std::cout << "Name: ";
    std::getline(std::cin, name);

    try {
        employeeService.addEmployee(Employee(id, name));
        std::cout << "\nEmployee added successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::viewEmployees() {
    printDivider();
    std::cout << "   ALL EMPLOYEES" << std::endl;
    printDivider();

    std::vector<Employee> employees = employeeService.getAllEmployees();

    if (employees.empty()) {
        std::cout << "No employees found." << std::endl;
    } else {
        std::cout << "ID\t\t| Name\t\t\t| Status" << std::endl;
        printDivider();
        for (Employee& e : employees) {
            std::cout << e.getUserId() << "\t| "
                      << e.getName() << "\t| "
                      << (e.isActiveUser() ? "Active" : "Inactive")
                      << std::endl;
        }
    }
    waitForEnter();
}

void ManagerConsole::editEmployee() {
    printDivider();
    std::cout << "   EDIT EMPLOYEE" << std::endl;
    printDivider();

    std::string id, name;
    std::cout << "Enter Employee ID to edit: ";
    std::getline(std::cin, id);
    std::cout << "New Name: ";
    std::getline(std::cin, name);

    try {
        employeeService.editEmployee(id, name);
        std::cout << "\nEmployee updated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::deactivateEmployee() {
    printDivider();
    std::cout << "   DEACTIVATE EMPLOYEE" << std::endl;
    printDivider();

    std::string id;
    std::cout << "Enter Employee ID to deactivate: ";
    std::getline(std::cin, id);

    try {
        std::vector<Order> orders = orderService.getAllOrders();
        employeeService.deactivateEmployee(id, orders);
        std::cout << "\nEmployee deactivated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::reactivateEmployee() {
    printDivider();
    std::cout << "   REACTIVATE EMPLOYEE" << std::endl;
    printDivider();

    std::string id;
    std::cout << "Enter Employee ID to reactivate: ";
    std::getline(std::cin, id);

    try {
        employeeService.reactivateEmployee(id);
        std::cout << "\nEmployee reactivated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

//------
// ORDER
//------

void ManagerConsole::handleOrders() {
    int choice;
    do {
        printDivider();
        std::cout << "   ORDER MANAGEMENT" << std::endl;
        printDivider();
        std::cout << "1. Create Order" << std::endl;
        std::cout << "2. View All Orders" << std::endl;
        std::cout << "3. Edit Order" << std::endl;
        std::cout << "4. Assign Order to Employee" << std::endl;
        std::cout << "0. Back" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 4);

        switch (choice) {
            case 1: createOrder(); break;
            case 2: viewOrders(); break;
            case 3: editOrder(); break;
            case 4: assignOrder(); break;
        }
    } while (choice != 0);
}

void ManagerConsole::createOrder() {
    printDivider();
    std::cout << "   CREATE ORDER" << std::endl;
    printDivider();

    std::string orderNumber;
    std::cout << "Order Number: ";
    std::getline(std::cin, orderNumber);

    Order newOrder(orderNumber);

    // Allow adding multiple items to the order
    std::string addMore = "y";
    while (addMore == "y" || addMore == "Y") {
        std::string productId;
        int quantity;

        std::cout << "Product ID: ";
        std::getline(std::cin, productId);
        std::cout << "Quantity: ";
        std::cin >> quantity;
        std::cin.ignore();

        newOrder.addItem(OrderItem(productId, quantity));

        std::cout << "Add another item? (y/n): ";
        std::getline(std::cin, addMore);
    }

    try {
        orderService.createOrder(newOrder);
        std::cout << "\nOrder created successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::viewOrders() {
    printDivider();
    std::cout << "   ALL ORDERS" << std::endl;
    printDivider();

    std::vector<Order> orders = orderService.getAllOrders();

    if (orders.empty()) {
        std::cout << "No orders found." << std::endl;
    } else {
        for (Order& o : orders) {
            std::string statusStr;
            switch (o.getStatus()) {
                case OrderStatus::CREATED:   statusStr = "CREATED"; break;
                case OrderStatus::ASSIGNED:  statusStr = "ASSIGNED"; break;
                case OrderStatus::COMPLETED: statusStr = "COMPLETED"; break;
            }

            std::cout << "Order: " << o.getOrderNumber()
                      << " | Status: " << statusStr
                      << " | Employee: "
                      << (o.getAssignedEmployeeId().empty() ? "Unassigned" : o.getAssignedEmployeeId())
                      << std::endl;

            for (OrderItem& item : o.getItems()) {
                std::cout << "   - " << item.getProductId()
                          << " x" << item.getQuantity() << std::endl;
            }
        }
    }
    waitForEnter();
}

void ManagerConsole::editOrder() {
    printDivider();
    std::cout << "   EDIT ORDER" << std::endl;
    printDivider();

    std::string orderNumber;
    std::cout << "Enter Order Number to edit: ";
    std::getline(std::cin, orderNumber);

    std::vector<OrderItem> newItems;
    std::string addMore = "y";

    // Replacing all items - user enters the complete new item list
    std::cout << "Enter new items (this will replace all existing items):" << std::endl;
    while (addMore == "y" || addMore == "Y") {
        std::string productId;
        int quantity;

        std::cout << "Product ID: ";
        std::getline(std::cin, productId);
        std::cout << "Quantity: ";
        std::cin >> quantity;
        std::cin.ignore();

        newItems.push_back(OrderItem(productId, quantity));

        std::cout << "Add another item? (y/n): ";
        std::getline(std::cin, addMore);
    }

    try {
        orderService.editOrder(orderNumber, newItems);
        std::cout << "\nOrder updated successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

void ManagerConsole::assignOrder() {
    printDivider();
    std::cout << "   ASSIGN ORDER" << std::endl;
    printDivider();

    std::string orderNumber, employeeId;
    std::cout << "Order Number: ";
    std::getline(std::cin, orderNumber);
    std::cout << "Employee ID: ";
    std::getline(std::cin, employeeId);

    try {
        orderService.assignOrder(orderNumber, employeeId);
        std::cout << "\nOrder assigned successfully!" << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

//--------
// REPORTS
//--------

void ManagerConsole::handleReports() {
    int choice;
    do {
        printDivider();
        std::cout << "   REPORTS" << std::endl;
        printDivider();
        std::cout << "1. Export Inventory Report (CSV)" << std::endl;
        std::cout << "2. Export All Orders Report (CSV)" << std::endl;
        std::cout << "3. Export Completed Orders Report (CSV)" << std::endl;
        std::cout << "4. Export Pending Orders Report (CSV)" << std::endl;
        std::cout << "0. Back" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 4);

        try {
            switch (choice) {
                case 1:
                    inventoryService.exportReport("data/inventory.csv");
                    std::cout << "\nInventory report saved to data/inventory.csv" << std::endl;
                    waitForEnter();
                    break;
                case 2:
                    orderService.exportReport("data/all_orders.csv", "ALL");
                    std::cout << "\nAll orders report saved to data/all_orders.csv" << std::endl;
                    waitForEnter();
                    break;
                case 3:
                    orderService.exportReport("data/completed_orders.csv", "COMPLETED");
                    std::cout << "\nCompleted orders report saved to data/completed_orders.csv." << std::endl;
                    waitForEnter();
                    break;
                case 4:
                    orderService.exportReport("data/pending_orders.csv", "PENDING");
                    std::cout << "\nPending orders report saved to data/pending_orders.csv." << std::endl;
                    waitForEnter();
                    break;
            }
        } catch (std::runtime_error& e) {
            std::cout << "\nError: " << e.what() << std::endl;
            waitForEnter();
        }
    } while (choice != 0);
}
