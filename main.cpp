#define CROW_MAIN
#define CROW_ENABLE_PATH_PARAMS

#include "crow_all.h"
#include "CartManager.h"
#include "InventoryManager.h"
#include <iostream>
using namespace std;


int main()
{
	crow::SimpleApp app;
	CartManager cartManager;
	InventoryManager inventoryManager("../Products.json");

	CROW_ROUTE(app, "/")
	([]() {
		std::ifstream file("../public/index.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	});

	CROW_ROUTE(app, "/cart")
	([]() {
		std::ifstream file("../public/cart.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	});

	CROW_ROUTE(app, "/sneaker/<int>")
	([](int id) {
		std::ifstream file("../public/sneaker.html");
		if (!file.is_open()) {
			return crow::response(404);
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		auto page = buffer.str();
		return crow::response(page);
	});


	CROW_ROUTE(app, "/api/cart/set").methods("POST"_method)
	([&](const crow::request& req) {
		crow::response res;

		bool success = cartManager.addToCart(req, res);
		if(success){
			res.code = 200;
			res.write("Item added to cart.");
			return res;
		} else {
			res.code = 400;
			res.write("Couldnt Add Item to Cart.");
			return res;
		}
	});

	CROW_ROUTE(app, "/api/cart").methods("GET"_method)
	([&](const crow::request& req){

		crow::json::wvalue json;
		cartManager.getCart(req, json);

		return json;
	});

	CROW_ROUTE(app, "/api/inventory").methods("GET"_method)
	([&](const crow::request& req){

		crow::json::wvalue json;
		inventoryManager.getInventory(json);

		return json;
	});


	CROW_ROUTE(app, "/styles/<path>")
	([](const std::string& filepath){
		std::string path = "../public/styles/" + filepath;
		std::ifstream file(path);
		if (!file.is_open()) return crow::response(404);

		std::stringstream buffer;
		buffer << file.rdbuf();

		crow::response res;
		res.set_header("Content-Type", "text/css");
		res.write(buffer.str());
		return res;
	});

	CROW_ROUTE(app, "/scripts/<path>")
	([](const std::string& filepath){
		std::string path = "../public/scripts/" + filepath;
		std::ifstream file(path);
		if (!file.is_open()) return crow::response(404);

		std::stringstream buffer;
		buffer << file.rdbuf();

		crow::response res;
		res.set_header("Content-Type", "application/javascript");
		res.write(buffer.str());
		return res;
	});


	CROW_ROUTE(app, "/images/<path>")
	([](const std::string& filepath){
		std::string path = "../public/images/" + filepath;
		std::ifstream file(path, std::ios::binary); // binary for images!
		if (!file.is_open()) return crow::response(404);
	
		std::stringstream buffer;
		buffer << file.rdbuf();
	
		crow::response res;
	
		// Set correct Content-Type
		if (filepath.ends_with(".jpg") || filepath.ends_with(".jpeg"))
			res.set_header("Content-Type", "image/jpeg");
		else if (filepath.ends_with(".png"))
			res.set_header("Content-Type", "image/png");
	
		res.body = buffer.str();
		res.code = 200;
		return res;
	});
	

	app.port(23500).multithreaded().run();
	return 1;
}