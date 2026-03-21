#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include <vector>
#include <string>
#include "Order.h"

// Handles file I/O operations for Order data using CSV format.
class OrderRepository {
private:
    std::string filePath;

public:
    // Initialises repo with file path.
    OrderRepository(std::string filePath);

    // Loads orders and item lists from CSV file.
    std::vector<Order> loadAll();

    // Saves the list of orders to CSV file.
    void saveAll(std::vector<Order> orders);

    // Searches for order by order number.
    Order* findByNumber(std::vector<Order>& orders, std::string orderNumber);

    // Filters and returns orders assigned to employee ID.
    std::vector<Order> findByEmployee(std::vector<Order>& orders, std::string employeeId);
};

#endif