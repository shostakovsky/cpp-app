#include "Config.h"
#include <format>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string_view>

namespace LifeExe
{

namespace
{
const nlohmann::json& require(const nlohmann::json& object, std::string_view key)
{
    if (!object.contains(key))
    {
        throw std::runtime_error(std::format("config: key '{}' is missing", key));
    }

    return object.at(key);
}
}  // namespace

AppConfig parseConfig(std::istream& input)
{
    AppConfig config;

    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(input);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        throw std::runtime_error(std::format("config: parse error: {}", e.what()));
    }

    try
    {
        config.title = require(json, "title").get<std::string>();
        const auto& window = require(json, "window");
        config.width = require(window, "width").get<int>();
        config.height = require(window, "height").get<int>();
    }
    catch (const nlohmann::json::type_error& e)
    {
        throw std::runtime_error(std::format("config: type error: {}", e.what()));
    }

    return config;
}
}  // namespace LifeExe