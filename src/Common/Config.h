#pragma once

#include <istream>
#include <string>

/// Common game logic: structures and parsing utilities.
namespace LifeExe
{

/// Game window and startup configuration.
struct GameConfig
{
    std::string title;  ///< Window caption title.
    int width{0};       ///< Window width in pixels.
    int height{0};      ///< Window height in pixels.
};

/// Reads game configuration from a JSON stream.
///
/// Expected format: `{"title": "...", "window": {"width": 1280, "height": 720}}`.
///
/// @param input Open stream containing valid JSON data.
/// @return Parsed GameConfig instance.
/// @throws std::runtime_error If JSON syntax is invalid or required keys are missing.
/// @note In case of missing keys, exception message indicates the problematic key name.
GameConfig parseConfig(std::istream& input);

}  // namespace LifeExe
