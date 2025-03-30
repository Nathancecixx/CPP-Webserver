#ifndef CART_MANAGER_H
#define CART_MANAGER_H

#include "crow_all.h"
#include <unordered_map>
#include <vector>
#include <string>

struct CartItem {
    int id;
    int quantity;
};

using Cart = std::vector<CartItem>;

class CartManager {
    std::unordered_map<std::string, Cart> carts;

    public:
    bool addToCart(const crow::request& req, crow::response& res);
    bool getCart(const crow::request& req, crow::json::wvalue& json);
    std::string readCartFile(const std::string& sessionId);
    std::string getSessionId(const crow::request& req);

    private:
    std::string getOrCreateSessionId(const crow::request& req, crow::response& res);

    bool saveCartToFile(const std::string& sessionId);
    bool loadCartFromFile(const std::string& sessionId);

};

#endif // CART_MANAGER_H