#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "OrderItem.h"
#include "OrderStatus.h"

// Represents customer order containing items and status.
class Order {
private:
    std::string orderNumber;        
    OrderStatus status;             
    std::string assignedEmployeeId; 
    std::vector<OrderItem> items;  

public:
    // Creates new order in default state (Created).
    Order(std::string orderNumber);

    std::string getOrderNumber() const;
    OrderStatus getStatus() const;
    std::string getAssignedEmployeeId() const;
    std::vector<OrderItem> getItems() const;

    // Updates order status.
    void setStatus(OrderStatus newStatus);

    // Links order to employee and updates status to (Assigned).
    void assignTo(std::string employeeId);

    // Adds new product item to order list.
    void addItem(OrderItem item);

    // Removes item from order based on product ID.
    void removeItem(std::string productId);
};

#endif