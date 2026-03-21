#include "OrderService.h"
#include <fstream>
#include <stdexcept>

// Repos required for organising order operations.
OrderService::OrderService(OrderRepository& orderRepo,
                           ProductRepository& productRepo,
                           EmployeeRepository& employeeRepo)
    : orderRepo(orderRepo),
      productRepo(productRepo),
      employeeRepo(employeeRepo) {}

// Checks if order number is present in list.
bool OrderService::orderExists(std::string orderNumber, std::vector<Order>& orders) {
    for (Order& o : orders) {
        if (o.getOrderNumber() == orderNumber) return true;
    }
    return false;
}

// Verifies all items in the order have stock.
bool OrderService::validateStock(Order& order, std::vector<Product>& products) {
    for (OrderItem& item : order.getItems()) {
        Product* p = productRepo.findById(products, item.getProductId());
        if (p == nullptr) {
            throw std::runtime_error("Product not found: " + item.getProductId());
        }
        if (p->getQuantity() < item.getQuantity()) {
            return false;
        }
    }
    return true;
}

// Adds new order to system after validation.
void OrderService::createOrder(Order order) {
    std::vector<Order> orders = orderRepo.loadAll();

    if (orderExists(order.getOrderNumber(), orders)) {
        throw std::runtime_error("Order number already exists: " + order.getOrderNumber());
    }

    if (order.getItems().empty()) {
        throw std::runtime_error("Can't create an order with no items.");
    }

    orders.push_back(order);
    orderRepo.saveAll(orders);
}

// Replaces item list of order that hasn't been completed.
void OrderService::editOrder(std::string orderNumber, std::vector<OrderItem> newItems) {
    std::vector<Order> orders = orderRepo.loadAll();
    Order* o = orderRepo.findByNumber(orders, orderNumber);

    if (o == nullptr || o->getStatus() == OrderStatus::COMPLETED) {
        throw std::runtime_error("Order not found or already completed.");
    }

    if (newItems.empty()) {
        throw std::runtime_error("Order must have at least one item.");
    }

    std::vector<OrderItem> existing = o->getItems();
    for (OrderItem& item : existing) {
        o->removeItem(item.getProductId());
    }
    for (OrderItem& item : newItems) {
        o->addItem(item);
    }

    orderRepo.saveAll(orders);
}

// Assigns order to active employee -> updates data stores.
void OrderService::assignOrder(std::string orderNumber, std::string employeeId) {
    std::vector<Order> orders = orderRepo.loadAll();
    Order* o = orderRepo.findByNumber(orders, orderNumber);

    if (o == nullptr || o->getStatus() == OrderStatus::COMPLETED) {
        throw std::runtime_error("Order not found or already completed.");
    }

    std::vector<Employee> employees = employeeRepo.loadAll();
    Employee* emp = employeeRepo.findById(employees, employeeId);

    if (emp == nullptr || !emp->isActiveUser()) {
        throw std::runtime_error("Employee not found or inactive.");
    }

    o->assignTo(employeeId);
    orderRepo.saveAll(orders);

    emp->addOrderId(orderNumber);
    employeeRepo.saveAll(employees);
}

// Finalises order by reducing stock and updating statuses across system.
void OrderService::completeOrder(std::string orderNumber) {
    std::vector<Order> orders = orderRepo.loadAll();
    Order* o = orderRepo.findByNumber(orders, orderNumber);

    if (o == nullptr || o->getStatus() != OrderStatus::ASSIGNED) {
        throw std::runtime_error("Order must be assigned to be completed.");
    }

    std::vector<Product> products = productRepo.loadAll();

    if (!validateStock(*o, products)) {
        throw std::runtime_error("Insufficient stock to complete order.");
    }

    for (OrderItem& item : o->getItems()) {
        Product* p = productRepo.findById(products, item.getProductId());
        p->reduceStock(item.getQuantity());
    }

    o->setStatus(OrderStatus::COMPLETED);
    productRepo.saveAll(products);
    orderRepo.saveAll(orders);

    std::vector<Employee> employees = employeeRepo.loadAll();
    Employee* emp = employeeRepo.findById(employees, o->getAssignedEmployeeId());
    if (emp != nullptr) {
        emp->removeOrderId(orderNumber);
        employeeRepo.saveAll(employees);
    }
}

// Retrieves orders stored in system.
std::vector<Order> OrderService::getAllOrders() {
    return orderRepo.loadAll();
}

// Retrieves orders assigned to employee ID.
std::vector<Order> OrderService::getOrdersByEmployee(std::string employeeId) {
    std::vector<Order> orders = orderRepo.loadAll();
    return orderRepo.findByEmployee(orders, employeeId);
}

// Returns list of orders not marked as completed.
std::vector<Order> OrderService::getPendingOrders() {
    std::vector<Order> all = orderRepo.loadAll();
    std::vector<Order> pending;
    for (Order& o : all) {
        if (o.getStatus() != OrderStatus::COMPLETED) {
            pending.push_back(o);
        }
    }
    return pending;
}

// Returns list of orders completed.
std::vector<Order> OrderService::getCompletedOrders() {
    std::vector<Order> all = orderRepo.loadAll();
    std::vector<Order> completed;
    for (Order& o : all) {
        if (o.getStatus() == OrderStatus::COMPLETED) {
            completed.push_back(o);
        }
    }
    return completed;
}

// Generates CSV report of orders based on status filter.
void OrderService::exportReport(std::string filePath, std::string filter) {
    std::vector<Order> orders;

    if (filter == "COMPLETED") orders = getCompletedOrders();
    else if (filter == "PENDING") orders = getPendingOrders();
    else orders = getAllOrders();

    std::ofstream file(filePath);
    if (!file.is_open()) throw std::runtime_error("Could not create report file.");

    file << "OrderNumber,Status,AssignedEmployee,Items\n";

    for (Order& o : orders) {
        std::string statusStr = (o.getStatus() == OrderStatus::CREATED) ? "CREATED" : 
                                (o.getStatus() == OrderStatus::ASSIGNED) ? "ASSIGNED" : "COMPLETED";

        std::string itemsStr;
        for (size_t i = 0; i < o.getItems().size(); i++) {
            itemsStr += o.getItems()[i].getProductId() + ":" + std::to_string(o.getItems()[i].getQuantity());
            if (i < o.getItems().size() - 1) itemsStr += "|";
        }

        file << o.getOrderNumber() << "," << statusStr << "," << o.getAssignedEmployeeId() << "," << itemsStr << "\n";
    }
    file.close();
}