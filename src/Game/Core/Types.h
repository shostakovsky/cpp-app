#pragma once

#include <cstdint>
#include <string_view>

namespace LifeExe::Core
{
/// Identifies the lifecycle state of the game.
enum class GameState : std::uint8_t
{
    /// The game simulation is running.
    Playing = 0,
    /// The game simulation is suspended.
    Paused,
    /// The game session has ended.
    GameOver
};

/// Converts a game state to its display name.
/// @param state Game state to convert.
/// @return String representation of the state.
constexpr std::string_view toString(GameState state)
{
    switch (state)
    {
        case GameState::Playing: return "Playing";
        case GameState::Paused: return "Paused";
        case GameState::GameOver: return "GameOver";
        default: return "Unknown";
    }
}

}  // namespace LifeExe::Core
