// Employee class
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "User.h"
#include <vector>


// Represents employee user.
class Employee : public User {
private:
    // List of order numbers currently assigned to this employee
    std::vector<std::string> assignedOrderIds;

public:
    // Constructs Employee with details given.
    Employee(std::string userId, std::string name, bool isActive = true);

    // Returns role for user.
    std::string getRole() const override;

    // Returns list of order IDs assigned to employee.
    std::vector<std::string> getAssignedOrderIds() const;

    // Adds order ID to employee's assigned list.
    void addOrderId(std::string orderId);

    // Removes order ID from employee's assigned list.
    void removeOrderId(std::string orderId);
};

#endif
