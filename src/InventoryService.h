#ifndef INVENTORYSERVICE_H
#define INVENTORYSERVICE_H

#include <vector>
#include <string>
#include "Product.h"
#include "Order.h"
#include "ProductRepository.h"

// Business logic for inventory and stock management.
class InventoryService {
private:
    ProductRepository& productRepo; // Refers to product data layer.

    // Checks if product ID is registered in system.
    bool idExists(std::string productId, std::vector<Product>& products);

public:
    // Adds product repository dependency into service.
    InventoryService(ProductRepository& productRepo);

    // Adds new product to system.
    void addProduct(Product product);

    // Updates existing product details -> saves changes to storage.
    void editProduct(std::string productId, Product updatedProduct);

    // Makes product inactive -> if not part of any pending orders.
    void deactivateProduct(std::string productId, std::vector<Order>& orders);

    // Restores deactivated product to active.
    void reactivateProduct(std::string productId);

    // Searches for products.
    std::vector<Product> searchByName(std::string query);

    // Retrieves product by id.
    Product* findById(std::string productId);

    // Generates CSV report containing state of items.
    void exportReport(std::string filePath);
};

#endif