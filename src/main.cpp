#define CROW_MAIN
#define CROW_ENABLE_PATH_PARAMS

#include "crow_all.h"
#include "CartManager.h"
#include "InventoryManager.h"
#include "AuthManager.h"
#include <iostream>
using namespace std;


crow::response serveFile(const std::string& path) {
	//Open file to serve
	std::ifstream file(path, std::ios::in);
	if (!file.is_open()) {
		return crow::response(404, "File Not Found");
	}
	//Copy it into a buffer
	std::stringstream buffer;
	buffer << file.rdbuf();
	//Return the buffer in the body of a crow response
	return crow::response(buffer.str());
}

int main()
{
	crow::SimpleApp app;

	InventoryManager inventoryManager("../Products.json");
	CartManager cartManager([&inventoryManager](int id) -> std::string {
        return inventoryManager.getNameFromId(id);
    });
	AuthManager authManager("Admin", "Admin");

	//Page Routes
	CROW_ROUTE(app, "/")
	([]() {
		return serveFile("../public/index.html");
	});
	CROW_ROUTE(app, "/cart")
	([]() {
		return serveFile("../public/cart.html");
	});
	CROW_ROUTE(app, "/sneaker/<int>")
	([](int id) {
		return serveFile("../public/sneaker.html");
	});
	CROW_ROUTE(app, "/checkout")
	([]() {
		auto temp = serveFile("../public/checkout.html");
		temp.code = 402; // Payment Required
		return temp;
	});



	//REST api routes
	CROW_ROUTE(app, "/api/auth").methods("POST"_method)
	([&](const crow::request& req, crow::response& res) {
		//Get the username and password
		std::string username = req.url_params.get("Username");
		std::string password = req.url_params.get("Password");

		if(username.empty() || password.empty()){
			res.code = 400;
			res.write("Username or Password Missing.");
			res.end();
			return;
		}

		//Verify the username and password with our saved accounts
		int accountId = authManager.verify(username, password);

		if(accountId != -1){
			res.code = 202;
			res.write("Login Successful!");
		} else {
			res.code = 401;
			res.write("Invalid Credentials.");
		}
		res.end();
		return;
	});

	CROW_ROUTE(app, "/api/cart/reciept").methods("POST"_method)
	([&](const crow::request& req, crow::response& res) {
		//Check if session is valid
		std::string sessionId = cartManager.getSessionId(req);
		if(sessionId.empty()){
			res.code = 400;
			res.write("Session not found.");
			res.end();
			return;
		}
		//Get the reciept rext and send it back as a plain text file
		std::string receiptText = cartManager.readCartFile(sessionId);
		res.set_header("Content-Type", "text/plain");
		res.write(receiptText);
		res.end();
		return;
	});

	CROW_ROUTE(app, "/api/cart/set").methods("POST"_method)
	([&](const crow::request& req, crow::response& res) {
		//Send the request to cart manager
		bool success = cartManager.addToCart(req, res);
		if(success){
			res.code = 200;
			res.write("Item added to cart.");
		} else {
			res.code = 400;
			res.write("Couldnt Add Item to Cart.");
		}
		res.end();
		return;
	});

	CROW_ROUTE(app, "/api/cart").methods("GET"_method)
	([&](const crow::request& req) {

		//Create a crow json object and 
		//copy the cart into it
		crow::json::wvalue json;
		cartManager.getCart(req, json);

		return json;
	});

	CROW_ROUTE(app, "/api/inventory").methods("GET"_method)
	([&](const crow::request& req) {

		//Create a crow json object and 
		//copy the inventory into it
		crow::json::wvalue json;
		inventoryManager.getInventory(json);

		return json;
	});





	//Routes for css, js, and image files used by pages
	CROW_ROUTE(app, "/styles/<path>")
	([](const std::string& filepath) {
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
	([](const std::string& filepath) {
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
	([](const std::string& filepath) {
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