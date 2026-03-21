// EmployeeConsole class.
#include "EmployeeConsole.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>


// Constructs EmployeeConsole with logged-in employee's profile.
EmployeeConsole::EmployeeConsole(OrderService& orderService,
                                 InventoryService& inventoryService,
                                 Employee currentEmployee)
    : orderService(orderService),
      inventoryService(inventoryService),
      currentEmployee(currentEmployee) {}


//----------------
// UTILITY METHODS
//----------------

// Prints divider line for visual clarity.
void EmployeeConsole::printDivider() {
    std::cout << "----------------------------------------" << std::endl;
}


// Pauses until user presses Enter.
void EmployeeConsole::waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}


// Reads and validates menu choice within [min, max].
int EmployeeConsole::getMenuChoice(int min, int max) {
    int choice;
    // Loops when input invalid
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

// Displays main employee menu with employee's name.
void EmployeeConsole::showMainMenu() {
    int choice;
    do {
        printDivider();
        std::cout << "   EMPLOYEE MENU - Welcome, " << currentEmployee.getName() << std::endl;
        printDivider();
        std::cout << "1. View Assigned Orders" << std::endl;
        std::cout << "2. Find Product" << std::endl;
        std::cout << "3. Complete Order" << std::endl;
        std::cout << "4. Generate Reports" << std::endl;
        std::cout << "0. Logout" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 4);

        switch (choice) {
            case 1: viewAssignedOrders(); break;
            case 2: findProduct(); break;
            case 3: completeOrder(); break;
            case 4: handleReports(); break;
            case 0: std::cout << "Logging out..." << std::endl; break;
        }
    } while (choice != 0);
}

//---------------------
// VIEW ASSIGNED ORDERS
//---------------------

// Displays orders assigned to employee. 
void EmployeeConsole::viewAssignedOrders() {
    printDivider();
    std::cout << "ASSIGNED ORDERS" << std::endl;
    printDivider();

    // Filter orders to show only those assigned to current employee
    std::vector<Order> orders = orderService.getOrdersByEmployee(currentEmployee.getUserId());

    if (orders.empty()) {
        std::cout << "You have no assigned orders." << std::endl;
    } else {
        for (Order& o : orders) {
            std::string statusStr;
            switch (o.getStatus()) {
                case OrderStatus::CREATED:   statusStr = "CREATED"; break;
                case OrderStatus::ASSIGNED:  statusStr = "ASSIGNED"; break;
                case OrderStatus::COMPLETED: statusStr = "COMPLETED"; break;
            }

            std::cout << "Order: " << o.getOrderNumber()
                      << " | Status: " << statusStr << std::endl;

            std::cout << "   Items:" << std::endl;
            for (OrderItem& item : o.getItems()) {
                // Searches location for item to show employee where to find it
                std::vector<Product> allProducts = inventoryService.getAllProducts();
                std::string location = "Unknown";
                for (Product& p : allProducts) {
                    if (p.getProductId() == item.getProductId()) {
                        location = p.getLocation();
                        break;
                    }
                }
                std::cout << "   - " << item.getProductId()
                          << " x" << item.getQuantity()
                          << " (Location: " << location << ")"
                          << std::endl;
            }
            printDivider();
        }
    }
    waitForEnter();
}

//-------------
// FIND PRODUCT
//-------------

// Allows for two product search modes: exact ID or name.
void EmployeeConsole::findProduct() {
    printDivider();
    std::cout << "   FIND PRODUCT" << std::endl;
    printDivider();
    std::cout << "1. Search by Product ID" << std::endl;
    std::cout << "2. Search by Name" << std::endl;
    std::cout << "0. Back" << std::endl;
    printDivider();

    int choice = getMenuChoice(0, 2);
    if (choice == 0) return;

    std::string query;

    if (choice == 1) {
        // Exact product ID search
        std::cout << "Enter Product ID: ";
        std::getline(std::cin, query);

        std::vector<Product> allProducts = inventoryService.getAllProducts();
        bool found = false;
        for (Product& p : allProducts) {
            if (p.getProductId() == query) {
                printDivider();
                std::cout << "ID:       " << p.getProductId() << std::endl;
                std::cout << "Name:     " << p.getName() << std::endl;
                std::cout << "Quantity: " << p.getQuantity() << std::endl;
                std::cout << "Location: " << p.getLocation() << std::endl;
                std::cout << "Status:   " << (p.isActiveProduct() ? "Active" : "Inactive") << std::endl;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "No product found with specified ID: " << query << std::endl;
        }

    } else {
        // Partial case-insensitive name search
        std::cout << "Enter product name : ";
        std::getline(std::cin, query);

        std::vector<Product> results = inventoryService.searchByName(query);

        if (results.empty()) {
            std::cout << "No products found matching: " << query << std::endl;
        } else {
            printDivider();
            std::cout << "Results for \"" << query << "\":" << std::endl;
            printDivider();
            for (Product& p : results) {
                std::cout << "ID: " << p.getProductId()
                          << " | Name: " << p.getName()
                          << " | Qty: " << p.getQuantity()
                          << " | Location: " << p.getLocation()
                          << std::endl;
            }
        }
    }
    waitForEnter();
}

//---------------
// COMPLETE ORDER
//---------------

// Allows employee to mark assigned orders as complete.
void EmployeeConsole::completeOrder() {
    printDivider();
    std::cout << "   COMPLETE ORDER" << std::endl;
    printDivider();

    std::vector<Order> orders = orderService.getOrdersByEmployee(currentEmployee.getUserId());

    if (orders.empty()) {
        std::cout << "You have no assigned orders to complete." << std::endl;
        waitForEnter();
        return;
    }

    // Shows orders in ASSIGNED state
    std::cout << "Your assigned orders:" << std::endl;
    for (Order& o : orders) {
        if (o.getStatus() == OrderStatus::ASSIGNED) {
            std::cout << "  - " << o.getOrderNumber() << std::endl;
        }
    }
    printDivider();

    std::string orderNumber;
    std::cout << "Enter Order Number to complete: ";
    std::getline(std::cin, orderNumber);

    // Verifies that order number belongs to employee
    bool isAssigned = false;
    for (Order& o : orders) {
        if (o.getOrderNumber() == orderNumber) {
            isAssigned = true;
            break;
        }
    }

    if (!isAssigned) {
        std::cout << "\nError: Order not assigned to you." << std::endl;
        waitForEnter();
        return;
    }

    try {
        orderService.completeOrder(orderNumber); // Validates and reduces inventory
        std::cout << "\nOrder " << orderNumber << " completed successfully!" << std::endl;
        std::cout << "Stock count has been updated automatically." << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << "\nError: " << e.what() << std::endl;
    }
    waitForEnter();
}

//----------------
// REPORTS
//----------------

// Exports current employee's orders to CSV file.
void EmployeeConsole::handleReports() {
    int choice;
    do {
        printDivider();
        std::cout << "   REPORTS" << std::endl;
        printDivider();
        std::cout << "1. Export Completed Orders (CSV)" << std::endl;
        std::cout << "2. Export Pending Orders (CSV)" << std::endl;
        std::cout << "0. Back" << std::endl;
        printDivider();

        choice = getMenuChoice(0, 2);

        try {
            std::vector<Order> myOrders = orderService.getOrdersByEmployee(currentEmployee.getUserId());

            if (choice == 1 || choice == 2) {
                // File path for employee using ID
                std::string filePath = (choice == 1)
                    ? "data/report_emp_completed_" + currentEmployee.getUserId() + ".csv"
                    : "data/report_emp_pending_" + currentEmployee.getUserId() + ".csv";

                std::ofstream file(filePath);
                if (!file.is_open()) {
                    throw std::runtime_error("Could not create report file.");
                }

                file << "OrderNumber,Status,Items\n";
                for (Order& o : myOrders) {
                    bool isCompleted = (o.getStatus() == OrderStatus::COMPLETED);

                    // Skips orders not matching filter
                    if ((choice == 1 && !isCompleted) || (choice == 2 && isCompleted)) continue;

                    std::string statusStr = isCompleted ? "COMPLETED" : "ASSIGNED";

                    // Build items string
                    std::string itemsStr;
                    for (int i = 0; i < o.getItems().size(); i++) {
                        itemsStr += o.getItems()[i].getProductId() + ":" +
                                    std::to_string(o.getItems()[i].getQuantity());
                        if (i < o.getItems().size() - 1) itemsStr += "|";
                    }
                    file << o.getOrderNumber() << "," << statusStr << "," << itemsStr << "\n";
                }
                file.close();
                std::cout << "\nReport saved to " << filePath << std::endl;
                waitForEnter();
            }
        } catch (std::runtime_error& e) {
            std::cout << "\nError: " << e.what() << std::endl;
            waitForEnter();
        }
    } while (choice != 0);
}
