#define CROW_MAIN
#define CROW_ENABLE_PATH_PARAMS

#include "crow_all.h"
#include <iostream>
using namespace std;

int main()
{
	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
	([]() {
		std::ifstream file("../public/index.html");
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
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