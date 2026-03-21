#include "ProductRepository.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// Sets file path for product data persistence.
ProductRepository::ProductRepository(std::string filePath)
    : filePath(filePath) {}

// Parses CSV file to reconstruct product inventory list.
std::vector<Product> ProductRepository::loadAll() {
    std::vector<Product> products;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return products;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string productId, name, quantityStr, priceStr, location, isActiveStr;

        std::getline(ss, productId, ',');
        std::getline(ss, name, ',');
        std::getline(ss, quantityStr, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, location, ',');
        std::getline(ss, isActiveStr, ',');

        try {
            int qty = std::stoi(quantityStr);
            double price = std::stod(priceStr);
            bool active = (isActiveStr == "1");
            products.emplace_back(productId, name, qty, price, location, active);
        } catch (...) {
            continue; 
        }
    }
    return products;
}

// Writes current state of all products back to CSV file.
void ProductRepository::saveAll(std::vector<Product> products) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open products file for writing.");
    }

    for (const auto& p : products) {
        file << p.getProductId() << ","
             << p.getName() << ","
             << p.getQuantity() << ","
             << p.getPrice() << ","
             << p.getLocation() << ","
             << (p.isActiveProduct() ? "1" : "0")
             << "\n";
    }
}

// Performs linear search to find product by unique ID.
Product* ProductRepository::findById(std::vector<Product>& products, std::string id) {
    for (Product& p : products) {
        if (p.getProductId() == id) {
            return &p;
        }
    }
    return nullptr;
}

// Returns list of products.
std::vector<Product> ProductRepository::findByName(std::vector<Product>& products, std::string query) {
    std::vector<Product> results;
    std::transform(query.begin(), query.end(), query.begin(), ::tolower);

    for (Product& p : products) {
        std::string name = p.getName();
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);

        if (name.find(query) != std::string::npos) {
            results.push_back(p);
        }
    }
    return results;
}