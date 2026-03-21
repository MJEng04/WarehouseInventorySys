#ifndef USER_H
#define USER_H

#include <string>

// Base class providing basic attributes for system users.
class User {
protected:
    std::string userId; 
    std::string name;   
    bool isActive;     

public:
    // Constructs user with basic id details.
    User(std::string userId, std::string name, bool isActive = true);

    std::string getUserId() const;
    std::string getName() const;
    bool isActiveUser() const;

    void setName(std::string newName);
    void setActive(bool active);

    // Implemented by Manager and Employee subclasses.
    virtual std::string getRole() const = 0;

    // Ensures cleanup of derived classes.
    virtual ~User() {}
};

#endif