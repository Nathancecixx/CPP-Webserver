#include "InventoryManager.h"


InventoryManager::InventoryManager(const std::string& filepath){
    if (!loadInventory(filepath)) {
        std::cerr << "Failed to load inventory from " << filepath << std::endl;
    }
}

bool InventoryManager::loadInventory(const std::string& filepath) {
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open inventory file: " << filepath << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    infile.close();

    // Use Crow's JSON loader to parse the file contents
    auto jsonVal = crow::json::load(buffer.str());
    if (!jsonVal) {
        std::cerr << "Error: Failed to parse JSON from inventory file: " << filepath << std::endl;
        return false;
    }

    // Clear any existing inventory data
    inventory.clear();

    // Expect the JSON to be an array of items
    for (size_t i = 0; i < jsonVal.size(); i++) {
        const auto& item = jsonVal[i];
        InventoryItem invItem;
        invItem.id = item["id"].i();
        invItem.name = item["name"].s();
        invItem.price = static_cast<float>(item["price"].d());
        invItem.stock = item["stock"].i();
        invItem.image = item["img"].s();
        invItem.description = item["description"].s();
        inventory.push_back(invItem);
    }

    std::cout << "Successfully loaded " << inventory.size() << " inventory items." << std::endl;
    return true;
}

bool InventoryManager::getInventory(crow::json::wvalue& json) {
    // Create a JSON list to hold the inventory items.
    json = crow::json::wvalue::list();
    for (const auto& item : inventory) {
        crow::json::wvalue itemJson;
        itemJson["id"] = item.id;
        itemJson["name"] = item.name;
        itemJson["price"] = item.price;
        itemJson["stock"] = item.stock;
        itemJson["image"] = item.image;
        itemJson["description"] = item.description;
        // Use assignment to the next index to append.
        json[json.size()] = std::move(itemJson);
    }
    return true;
}

int InventoryManager::getItemId(int id) {
    // Example implementation: find the index of the inventory item with the matching id.
    for (size_t i = 0; i < inventory.size(); i++) {
        if (inventory[i].id == id) {
            return static_cast<int>(i);
        }
    }
    return -1; // Not found.
}