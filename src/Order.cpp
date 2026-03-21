#include "Order.h"
#include <algorithm>

// Initialises order members and sets status to (Created).
Order::Order(std::string orderNumber)
    : orderNumber(orderNumber), status(OrderStatus::CREATED),
      assignedEmployeeId("") {}

std::string Order::getOrderNumber() const { return orderNumber; }
OrderStatus Order::getStatus() const { return status; }
std::string Order::getAssignedEmployeeId() const { return assignedEmployeeId; }
std::vector<OrderItem> Order::getItems() const { return items; }

void Order::setStatus(OrderStatus newStatus) { status = newStatus; }

// Sets employee ID and order to (Assigned).
void Order::assignTo(std::string employeeId) {
    assignedEmployeeId = employeeId;
    status = OrderStatus::ASSIGNED;
}

// Adds product to items vector.
void Order::addItem(OrderItem item) {
    items.push_back(item);
}

// Filters items vector to remove matching product ID.
void Order::removeItem(std::string productId) {
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [&productId](const OrderItem& item) {
                return item.getProductId() == productId;
            }),
        items.end()
    );
}