#include "InventoryManager.h"


InventoryManager::InventoryManager(const std::string& filepath){
    //On creation load the inventory from static file
    if (!loadInventory(filepath)) {
        std::cerr << "Failed to load inventory from " << filepath << std::endl;
    }
}

bool InventoryManager::loadInventory(const std::string& filepath) {
    //Open file path
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open inventory file: " << filepath << std::endl;
        return false;
    }

    //Read contents into buffer
    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    //Use Crow's JSON loader to parse
    auto jsonVal = crow::json::load(buffer.str());
    if (!jsonVal) {
        std::cerr << "Error: Failed to parse JSON from inventory file: " << filepath << std::endl;
        return false;
    }

    //Clear any existing inventory data
    inventory.clear();

    //Create Inventory items with json data
    for (size_t i = 0; i < jsonVal.size(); i++) {
        const auto& item = jsonVal[i];
        InventoryItem invItem;
        invItem.id = item["id"].i();
        invItem.model = item["model"].s();
        invItem.name = item["name"].s();
        invItem.price = static_cast<float>(item["price"].d());
        invItem.last_sale = static_cast<float>(item["last_sale"].d());
        invItem.stock = item["stock"].i();
        invItem.image = item["image"].s();
        invItem.description = item["description"].s();
        invItem.release_date = item["release_date"].s();
        invItem.retail_price = static_cast<float>(item["retail_price"].d());
        inventory.push_back(invItem);
    }


    std::cout << "Successfully loaded " << inventory.size() << " inventory items." << std::endl;
    return true;
}

bool InventoryManager::getInventory(crow::json::wvalue& json) {
    //Create a json list to hold items
    json = crow::json::wvalue::list();
    for (const auto& item : inventory) {
        crow::json::wvalue itemJson;
        itemJson["id"] = item.id;
        itemJson["model"] = item.model;
        itemJson["name"] = item.name;
        itemJson["price"] = item.price;
        itemJson["last_sale"] = item.last_sale;
        itemJson["stock"] = item.stock;
        itemJson["image"] = item.image;
        itemJson["description"] = item.description;
        itemJson["release_date"] = item.release_date;
        itemJson["retail_price"] = item.retail_price;
        //Append the json object to the list
        json[json.size()] = std::move(itemJson);
    }
    return true;
}


std::string InventoryManager::getNameFromId(int id){
    //Find the id's correlated name
    for(const auto& item : inventory){
        if(item.id == id) return item.name;
    }
    return "";
}