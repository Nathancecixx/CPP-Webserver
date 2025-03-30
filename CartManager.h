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

    private:
    std::string getOrCreateSessionId(const crow::request& req, crow::response& res);
    std::string getSessionId(const crow::request& req);
};

#endif // CART_MANAGER_H