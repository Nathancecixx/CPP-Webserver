#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "crow_all.h"
#include <unordered_map>
#include <vector>
#include <string>

struct Account {
    int id;
    std::string name;
    std::string password;
};

class AuthManager {
    std::vector<Account> userAccounts;

    public:
    AuthManager() {
        userAccounts.push_back({1, "Admin", "Admin"});
    }
    AuthManager(const std::string& user, const std::string& password);
    int verify(const std::string& user, const std::string& password);

};

#endif // AUTH_MANAGER_H