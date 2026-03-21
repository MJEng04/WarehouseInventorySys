#include "OrderItem.h"

// Initialises order item with product and quantity.
OrderItem::OrderItem(std::string productId, int quantity)
    : productId(productId), quantity(quantity) {}

std::string OrderItem::getProductId() const { return productId; }
int OrderItem::getQuantity() const { return quantity; }
void OrderItem::setQuantity(int newQuantity) { quantity = newQuantity; }