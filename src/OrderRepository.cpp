#include "OrderRepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Initialises repo with path to order data file.
OrderRepository::OrderRepository(std::string filePath)
    : filePath(filePath) {}

// Maps status from file to OrderStatus.
static OrderStatus stringToStatus(const std::string& s) {
    if (s == "ASSIGNED")  return OrderStatus::ASSIGNED;
    if (s == "COMPLETED") return OrderStatus::COMPLETED;
    return OrderStatus::CREATED;
}

// Maps OrderStatus values to file.
static std::string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::ASSIGNED:  return "ASSIGNED";
        case OrderStatus::COMPLETED: return "COMPLETED";
        default:                     return "CREATED";
    }
}

// Analyses order CSV and reconstructs list of orders and items.
std::vector<Order> OrderRepository::loadAll() {
    std::vector<Order> orders;
    std::ifstream file(filePath);

    if (!file.is_open()) return orders;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string orderNum, statusStr, empId, itemsBatch;

        std::getline(ss, orderNum, ',');
        std::getline(ss, statusStr, ',');
        std::getline(ss, empId, ',');
        std::getline(ss, itemsBatch, ',');

        Order o(orderNum);
        if (empId != "NONE") o.assignTo(empId);
        o.setStatus(stringToStatus(statusStr));

        std::stringstream itemss(itemsBatch);
        std::string itemPair;
        while (std::getline(itemss, itemPair, '|')) {
            size_t colonPos = itemPair.find(':');
            if (colonPos != std::string::npos) {
                std::string pId = itemPair.substr(0, colonPos);
                int qty = std::stoi(itemPair.substr(colonPos + 1));
                o.addItem(OrderItem(pId, qty));
            }
        }
        orders.push_back(o);
    }
    return orders;
}

// Saves order data to CSV file.
void OrderRepository::saveAll(std::vector<Order> orders) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filePath);
    }

    for (Order& o : orders) {
        std::string empId = o.getAssignedEmployeeId().empty() ? "NONE" : o.getAssignedEmployeeId();
        file << o.getOrderNumber() << "," << statusToString(o.getStatus()) << "," << empId << ",";

        std::vector<OrderItem> items = o.getItems();
        for (int i = 0; i < items.size(); i++) {
            file << items[i].getProductId() << ":" << items[i].getQuantity();
            if (i < items.size() - 1) file << "|";
        }
        file << "\n";
    }
}

// Finds order within vector using order number.
Order* OrderRepository::findByNumber(std::vector<Order>& orders, std::string orderNumber) {
    for (Order& o : orders) {
        if (o.getOrderNumber() == orderNumber) return &o;
    }
    return nullptr;
}

// Filters to find list of orders assigned to employee.
std::vector<Order> OrderRepository::findByEmployee(std::vector<Order>& orders, std::string employeeId) {
    std::vector<Order> results;
    for (Order& o : orders) {
        if (o.getAssignedEmployeeId() == employeeId) {
            results.push_back(o);
        }
    }
    return results;
}