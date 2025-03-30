#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "crow_all.h"
#include <unordered_map>
#include <vector>
#include <string>

struct InventoryItem {
    int id;
    std::string name;
    float price;
    int stock;
    std::string image;
    std::string description;
};

class InventoryManager {

    private:
    std::vector<InventoryItem> inventory;

    public:

    InventoryManager(const std::string& filepath);

    bool getInventory(crow::json::wvalue& json);

    int getItemId(int id);

    private:
    bool loadInventory(const std::string& filepath);

};

#endif // INVENTORY_MANAGER_H