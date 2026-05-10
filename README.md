# Warehouse Inventory Management System

A console-based C++ application for managing warehouse stock and daily operations, 
featuring distinct role-based interfaces for managers and employees.

## Tech Stack
- **Language:** C++
- **Storage:** CSV file persistence
- **Architecture:** Four-layer (UI, Service, Repository, Domain)
- **Build:** GCC / G++

## Features

### Manager
- Product management — add, edit, deactivate/reactivate inventory items
- Employee management — register, edit, activate/deactivate staff
- Order management — create, assign, and track orders
- Report generation — export inventory and order data to CSV

### Employee
- View assigned orders with product locations
- Search products by ID or partial name
- Complete orders with automatic stock reduction
- Export personal order reports to CSV

## Architecture
The system follows a four-layer architecture enforcing clear separation of concerns:

- **Presentation Layer** — `ManagerConsole` and `EmployeeConsole` handle all user interaction
- **Service Layer** — `InventoryService`, `OrderService`, `EmployeeService`, `AuthService` contain business logic
- **Repository Layer** — `ProductRepository`, `OrderRepository`, `EmployeeRepository` manage CSV file I/O
- **Domain Layer** — `Product`, `Order`, `OrderItem`, `User`, `Employee`, `Manager` represent core entities

## How to Run

**Compile:**
```bash
g++ -o warehouse main.cpp src/*.cpp
```

**Run:**
```bash
./warehouse
```

**Default login:**
- Manager ID: `BIGBOSS004`
- Employee ID: see `data/employees.csv`

## Project Structure
warehouse-ims/
├── src/
│   ├── domain/     (User, Product, Order, OrderItem)
│   ├── services/   (Auth, Inventory, Order, Employee)
│   ├── repos/      (Product, Order, Employee repositories)
│   └── ui/         (Manager and Employee consoles)
├── data/
│   ├── products.csv
│   ├── employees.csv
│   └── orders.csv
├── main.cpp
└── README.md

## Key Design Decisions
- **Soft deletion** — products and employees are marked inactive rather than deleted, preserving historical order data
- **Order state machine** — orders follow a strict CREATED → ASSIGNED → COMPLETED lifecycle
- **Stock validation** — stock availability is checked before any order can be completed
- **Role-based access** — managers and employees see only their relevant functions