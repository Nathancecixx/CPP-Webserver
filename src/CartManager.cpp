#include <iostream>
#include "CartManager.h"

bool CartManager::addToCart(const crow::request& req, crow::response& res){
    //Get the session ID from the requests header
    std::string sessionId = getOrCreateSessionId(req, res);

    //Get the id and quantity from request body
    auto body = crow::json::load(req.body);
    if(!body) return false;
    int productId = body["id"].i();
    int quantity = body["quantity"].i();

    //Load current cart state from file
    loadCartFromFile(sessionId);

    //Retrieve the current cart state from memory
    auto& cart = carts[sessionId];


    //Either find the item and change its quanity
    //Or add a new item to cart with that quanity
    CartItem product;
    bool found = false;
    for(auto& item : cart){
        if(item.id == productId) {
            item.quantity = quantity;
            found = true;
            break;
        } 
    }
    if(!found) 
        cart.push_back({ productId, quantity });

    //Save the cart state to a file
    saveCartToFile(sessionId);
    return true;
};

bool CartManager::getCart(const crow::request& req, crow::json::wvalue& json){
    //Get the session id from the request header
    std::string sessionId = getSessionId(req);

    //Ensure cart state is correctly loaded from file
    loadCartFromFile(sessionId);
    
    //Get the current cart state
    Cart& cart = carts[sessionId];

    //Create a json struct through crow to contain the list of products
    json["cart"] = crow::json::wvalue::list();

    //Copy the in memory cart to json
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
        return "";
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


bool CartManager::saveCartToFile(const std::string& sessionId) {
    //Create/Open file to save cart too
    std::string filename = "cart_" + sessionId + ".txt";
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing." << std::endl;
        return false;
    }
    
    //Ensure cart state is correctly loaded from file
    loadCartFromFile(sessionId);

    //Find cart state from memory
    const auto& cart = carts[sessionId];

    //Write the id and quantity of products in cart to file
    for (const auto& item : cart) {
        ofs << item.id << " " << item.quantity << "\n";
    }
    
    ofs.close();
    return true;
}

bool CartManager::loadCartFromFile(const std::string& sessionId) {
    //Open file to load cart from
    std::string filename = "cart_" + sessionId + ".txt";
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        carts[sessionId] = Cart();
        return false;
    }
    
    //Construct cart by reading line by line
    Cart cart;
    int id, quantity;
    while (ifs >> id >> quantity) {
        cart.push_back({ id, quantity });
    }
    
    ifs.close();
    //Load the new cart struct into the in memory list
    carts[sessionId] = cart;
    return true;
}

std::string CartManager::readCartFile(const std::string& sessionId) {
    //open file to read cart from
    std::string filename = "cart_" + sessionId + ".txt";
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for reading." << std::endl;
        return "";
    }
    
    //Read the file line by line
    std::string line;
    std::stringstream output;
    while (std::getline(ifs, line)) {
        //Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        //Parse the line into id and quantity
        std::istringstream iss(line);
        int id, qty;
        if (!(iss >> id >> qty)) {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
        
        //Get the name of the current product id
        std::string product_name = this->getNameFromId(id);

        //Add product details to reciept to send back
        output << "Product id: " << id << ", " << product_name << ", qnty: " << qty << "\n";
    }
    ifs.close();
    return output.str();
}
