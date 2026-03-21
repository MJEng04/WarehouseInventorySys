#ifndef ORDERSERVICE_H
#define ORDERSERVICE_H

#include <vector>
#include <string>
#include "Order.h"
#include "Product.h"
#include "Employee.h"
#include "OrderRepository.h"
#include "ProductRepository.h"
#include "EmployeeRepository.h"

// Business logic for orders, products, and employees.
class OrderService {
private:
    OrderRepository& orderRepo;       // Repo for order data operations.
    ProductRepository& productRepo;   // Repo for product stock updates.
    EmployeeRepository& employeeRepo; // Repo for employee assignment tracking.

    // Checks if items in the order are available.
    bool validateStock(Order& order, std::vector<Product>& products);

    // Determines if order number is already registered in system.
    bool orderExists(std::string orderNumber, std::vector<Order>& orders);

public:
    // Required repos to manage order lifecycle.
    OrderService(OrderRepository& orderRepo,
                 ProductRepository& productRepo,
                 EmployeeRepository& employeeRepo);

    // Registers new order if order number is unique and items are valid.
    void createOrder(Order order);

    // Replaces items of existing order not been completed.
    void editOrder(std::string orderNumber, std::vector<OrderItem> newItems);

    // Links order to employee and updates records.
    void assignOrder(std::string orderNumber, std::string employeeId);

    // Reduces stock and marks order as finished after validation.
    void completeOrder(std::string orderNumber);

    // Retrieves order stored in system.
    std::vector<Order> getAllOrders();

    // Retrieves orders assigned to employee ID.
    std::vector<Order> getOrdersByEmployee(std::string employeeId);

    // Returns list of orders created or assigned.
    std::vector<Order> getPendingOrders();

    // Returns list of completed orders.
    std::vector<Order> getCompletedOrders();

    // Generates CSV file containing order data.
    void exportReport(std::string filePath, std::string filter = "ALL");
};

#endif