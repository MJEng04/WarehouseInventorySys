#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>

// Represents single product and quantity required within order.
class OrderItem {
private:
    std::string productId;
    int quantity;          

public:
    // Creates order item linking product ID to quantity.
    OrderItem(std::string productId, int quantity);

    // Retrieves product ID for this specific order line.
    std::string getProductId() const;

    // Retrieves quantity requested for item.
    int getQuantity() const;

    // Updates quantity of product for order.
    void setQuantity(int newQuantity);
};

#endif