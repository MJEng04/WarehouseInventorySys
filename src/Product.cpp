#include "Product.h"
#include <stdexcept>

// Initialises Product.
Product::Product(std::string productId, std::string name, int quantity,
                 double price, std::string location, bool isActive)
    : productId(productId), name(name), quantity(quantity),
      price(price), location(location), isActive(isActive) {}

std::string Product::getProductId() const { return productId; }
std::string Product::getName() const { return name; }
int Product::getQuantity() const { return quantity; }
double Product::getPrice() const { return price; }
std::string Product::getLocation() const { return location; }
bool Product::isActiveProduct() const { return isActive; }

void Product::setName(std::string newName) { name = newName; }
void Product::setQuantity(int newQuantity) { quantity = newQuantity; }
void Product::setPrice(double newPrice) { price = newPrice; }
void Product::setLocation(std::string newLocation) { location = newLocation; }
void Product::setActive(bool active) { isActive = active; }

// Reduce stock levels -> throws exception if requested amount is more than whats available.
void Product::reduceStock(int amount) {
    if (amount > quantity) {
        throw std::runtime_error("Insufficient stock for product: " + productId);
    }
    quantity -= amount;
}