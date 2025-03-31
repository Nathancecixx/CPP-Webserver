#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "crow_all.h"
#include <unordered_map>
#include <vector>
#include <string>

struct InventoryItem {
    int id;
    std::string model;
    std::string name;
    float price;
    float last_sale;
    int stock;
    std::string image;
    std::string description;
    std::string release_date;
    float retail_price;
};

class InventoryManager {

    private:
    std::vector<InventoryItem> inventory;

    public:
    InventoryManager(const std::string& filepath);

    bool getInventory(crow::json::wvalue& json);
    std::string getNameFromId(int id);

    private:
    bool loadInventory(const std::string& filepath);

};

#endif // INVENTORY_MANAGER_H