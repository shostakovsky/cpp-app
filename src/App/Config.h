#pragma once

#include <istream>
#include <string>

/// Application core logic: structures and parsing utilities.
namespace LifeExe
{

/// Application window and startup configuration.
struct AppConfig
{
    std::string title;  ///< Window caption title.
    int width{0};       ///< Window width in pixels.
    int height{0};      ///< Window height in pixels.
};

/// Reads application configuration from a JSON stream.
///
/// Expected format: `{"title": "...", "window": {"width": 1280, "height": 720}}`.
///
/// @param input Open stream containing valid JSON data.
/// @return Parsed AppConfig instance.
/// @throws std::runtime_error If JSON syntax is invalid or required keys are missing.
/// @note In case of missing keys, exception message indicates the problematic key name.
AppConfig parseConfig(std::istream& input);

}  // namespace LifeExe
