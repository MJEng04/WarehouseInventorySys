#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// Represents item with stock management.
class Product {
private:
    std::string productId; 
    std::string name;      
    int quantity;          
    double price;          
    std::string location;  
    bool isActive;        

public:
    // Initialises new product with provided details.
    Product(std::string productId, std::string name, int quantity,
            double price, std::string location, bool isActive = true);

    std::string getProductId() const;
    std::string getName() const;
    int getQuantity() const;
    double getPrice() const;
    std::string getLocation() const;
    bool isActiveProduct() const;

    void setName(std::string newName);
    void setQuantity(int newQuantity);
    void setPrice(double newPrice);
    void setLocation(std::string newLocation);
    void setActive(bool active);

    // Subtracts amount from stock after ensuring it's in stock.
    void reduceStock(int amount);
};

#endif