#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <nlohmann/json.hpp>

int main() {
	std::ifstream file("config.json");
	if (!file)
	{
		std::println(stderr, "Failed to open config.json");

		return EXIT_FAILURE;
	}

	try
	{
		auto json = nlohmann::json::parse(file);
		auto title = json.at("title").get<std::string>();
		auto window = json.at("window");
		auto width = window.at("width").get<int>();
		auto height = window.at("height").get<int>();

		std::println("Hello, {}! Window: {}x{}", title, width, height);
	}
	catch (const std::exception& e)
	{
		std::println(stderr, "Error reading config.json: {}", e.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
