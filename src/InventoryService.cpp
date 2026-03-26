#include "InventoryService.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

// Initialises service with reference to product repo.
InventoryService::InventoryService(ProductRepository& productRepo)
    : productRepo(productRepo) {}

// Goes through product list to verify if ID is already used.
bool InventoryService::idExists(std::string productId, std::vector<Product>& products) {
    for (Product& p : products) {
        if (p.getProductId() == productId) return true;
    }
    return false;
}

// Validates product ID before addidng it to data store.
void InventoryService::addProduct(Product product) {
    std::vector<Product> products = productRepo.loadAll();

    if (idExists(product.getProductId(), products)) {
        throw std::runtime_error("Product ID already exists: " + product.getProductId());
    }

    products.push_back(product);
    productRepo.saveAll(products);
}

std::vector<Product> InventoryService::getAllProducts() {
    return productRepo.loadAll();
}

std::vector<Product> InventoryService::getActiveProducts() {
    std::vector<Product> all = productRepo.loadAll();
    std::vector<Product> active;
    for (Product& p : all) {
        if (p.isActiveProduct()) active.push_back(p);
    }
    return active;
}

// FInds existing product by ID and applies updated attributes.
void InventoryService::editProduct(std::string productId, std::string newName,
                                    int newQuantity, double newPrice,
                                    std::string newLocation) {
    std::vector<Product> products = productRepo.loadAll();
    Product* p = productRepo.findById(products, productId);

    if (p == nullptr) {
        throw std::runtime_error("Product not found: " + productId);
    }
    if (newQuantity < 0) {
        throw std::runtime_error("Quantity cannot be negative.");
    }
    if (newPrice < 0) {
        throw std::runtime_error("Price cannot be negative.");
    }

    p->setName(newName);
    p->setQuantity(newQuantity);
    p->setPrice(newPrice);
    p->setLocation(newLocation);

    productRepo.saveAll(products);
}

// Prevents deactivation of products required for orders.
void InventoryService::deactivateProduct(std::string productId, std::vector<Order>& orders) {
    for (Order& o : orders) {
        if (o.getStatus() != OrderStatus::COMPLETED) {
            for (OrderItem& item : o.getItems()) {
                if (item.getProductId() == productId) {
                    throw std::runtime_error("Product is in a pending order and can't be deactivated.");
                }
            }
        }
    }

    std::vector<Product> products = productRepo.loadAll();
    Product* p = productRepo.findById(products, productId);
    if (p != nullptr) {
        p->setActive(false);
        productRepo.saveAll(products);
    }
}

// Sets active status flag of product back to true.
void InventoryService::reactivateProduct(std::string productId) {
    std::vector<Product> products = productRepo.loadAll();
    Product* p = productRepo.findById(products, productId);
    if (p != nullptr) {
        p->setActive(true);
        productRepo.saveAll(products);
    }
}

// Searches repo for products matching  search string.
std::vector<Product> InventoryService::searchByName(std::string query) {
    std::vector<Product> all = productRepo.loadAll();
    return productRepo.findByName(all, query);
}

// Returns pointer to product found by searching product list.
Product* InventoryService::findById(std::string productId) {
    std::vector<Product> products = productRepo.loadAll();
    return productRepo.findById(products, productId);
}

// Writes product data to CSV file for admin records.
void InventoryService::exportReport(std::string filePath) {
    std::vector<Product> products = productRepo.loadAll();
    std::ofstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not create report file: " + filePath);
    }

    file << "ProductID,Name,Quantity,Price,Location,Status\n";
    for (Product& p : products) {
        file << p.getProductId() << ","
             << p.getName() << ","
             << p.getQuantity() << ","
             << p.getPrice() << ","
             << p.getLocation() << ","
             << (p.isActiveProduct() ? "Active" : "Inactive") << "\n";
    }
    file.close();
}