#include <iostream>
#include "CartManager.h"

bool CartManager::addToCart(const crow::request& req, crow::response& res){
    std::string sessionId = getOrCreateSessionId(req, res);

    auto body = crow::json::load(req.body);
    if(!body) return false;

    int productId = body["id"].i();
    int quantity = body["quantity"].i();

    auto& cart = carts[sessionId];


    CartItem product;
    bool found = false;

    for(auto& item : cart){
        if(item.id == productId) {
            item.quantity = quantity;
            found = true;
            return true;
        } 
    }

    if(!found) 
        cart.push_back({ productId, quantity });

    return true;
};

bool CartManager::getCart(const crow::request& req, crow::json::wvalue& json){
    std::string sessionId = getSessionId(req);
    
    Cart& cart = carts[sessionId];

    json["cart"] = crow::json::wvalue::list();

    for(const auto& item : cart){
        crow::json::wvalue itemJson;
        itemJson["id"] = item.id;
        itemJson["quantity"] = item.quantity;
        json["cart"][json["cart"].size()] = std::move(itemJson);
    }

    return true;
};


std::string CartManager::getSessionId(const crow::request& req){
    auto cookieHeader = req.get_header_value("Cookie");
    std::string sessionId;

    //Parse session id from request
    auto pos = cookieHeader.find("session_id=");
	if (pos != std::string::npos) {
		sessionId = cookieHeader.substr(pos + 11);
	} else {
        return nullptr;
	}

    return sessionId;
}


std::string CartManager::getOrCreateSessionId(const crow::request& req, crow::response& res){
    auto cookieHeader = req.get_header_value("Cookie");
    std::string sessionId;

    //Parse session id from request
    auto pos = cookieHeader.find("session_id=");
	if (pos != std::string::npos) {
		sessionId = cookieHeader.substr(pos + 11);
	} else {
		//Create new session ID for response
		sessionId = std::to_string(std::rand());
		res.add_header("Set-Cookie", "session_id=" + sessionId);
	}

    return sessionId;
};