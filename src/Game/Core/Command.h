#pragma once
#include <cstdint>
#include <string_view>

namespace LifeExe::Core
{
/// Commands that can be applied to the game.
enum class Command : std::uint8_t
{
    None = 0,
    Pause,
    Resume,
    TogglePause,
    Reset,
    Quit
};

/// Returns the display name of a command.
/// @param command Command to convert.
constexpr std::string_view toString(Command command)
{
    switch (command)
    {
        case Command::None: return "None";
        case Command::Pause: return "Pause";
        case Command::Resume: return "Resume";
        case Command::TogglePause: return "TogglePause";
        case Command::Reset: return "Reset";
        case Command::Quit: return "Quit";
        default: return "Unknown";
    }
}
}  // namespace LifeExe::Core