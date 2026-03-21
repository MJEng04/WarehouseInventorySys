#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include <vector>
#include <string>
#include "Product.h"

// Provides file-based persistence for product data using CSV format.
class ProductRepository {
private:
    std::string filePath; // Location of products CSV file.

public:
    // Initialises repository with specified file path.
    ProductRepository(std::string filePath);

    // Loads products from CSV file into vector.
    std::vector<Product> loadAll();

    // Overwrites CSV file with list of products.
    void saveAll(std::vector<Product> products);

    // Searches vector for product matching given ID.
    Product* findById(std::vector<Product>& products, std::string id);

    // Filters products by name using case-insensitive partial match.
    std::vector<Product> findByName(std::vector<Product>& products, std::string query);
};

#endif