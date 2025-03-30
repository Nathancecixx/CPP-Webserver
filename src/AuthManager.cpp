#include "AuthManager.h"

AuthManager::AuthManager(const std::string& user, const std::string& password){
    userAccounts.push_back({1, user, password});
}

int AuthManager::verify(const std::string& user, const std::string& password){
    
    for (const auto &account : userAccounts) {
        if (account.name == user && account.password == password) {
            return account.id;
        }
    }
    return -1;
}