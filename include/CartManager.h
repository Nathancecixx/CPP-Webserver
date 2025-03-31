#ifndef CART_MANAGER_H
#define CART_MANAGER_H

#include "crow_all.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

struct CartItem {
    int id;
    int quantity;
};

//Define Cart as a vector of cart items
using Cart = std::vector<CartItem>;

//Function definition for lambda function from the Inventory Manager
using InventoryFunc = std::function<std::string(int)>;

class CartManager {
    std::unordered_map<std::string, Cart> carts;

    public:
    CartManager(InventoryFunc func) : getNameFromId(func) {}

    bool addToCart(const crow::request& req, crow::response& res);
    bool getCart(const crow::request& req, crow::json::wvalue& json);
    std::string readCartFile(const std::string& sessionId);
    std::string getSessionId(const crow::request& req);

    private:
    InventoryFunc getNameFromId;
    
    std::string getOrCreateSessionId(const crow::request& req, crow::response& res);

    bool saveCartToFile(const std::string& sessionId);
    bool loadCartFromFile(const std::string& sessionId);

};

#endif // CART_MANAGER_H