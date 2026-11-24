#pragma once

#include "GameSnapshot.h"
#include "Command.h"

namespace LifeExe::Core
{
/// Owns and updates the current game state.
class Game
{
public:
    /// Constructs a game in the initial playing state.
    Game() = default;

    /// Advances the game simulation by the given delta time.
    /// @param dt Time elapsed since the previous update.
    void update(float dt);

    /// Returns the current game state.
    GameState state() const { return m_state; }

    /// Returns the elapsed game time.
    double totalTime() const { return m_totalTime; }

    /// Returns a copy of the game state for rendering.
    GameSnapshot snapshot() const { return GameSnapshot{.state = m_state, .totalTime = m_totalTime}; }

    /// Applies a command to the game.
    /// @param command Command to apply.
    void handleCommand(Command command);

    /// Restores the game to its initial playing state.
    void reset();

private:
    GameState m_state{GameState::Playing};
    double m_totalTime{0.0};
};
}  // namespace LifeExe::Core
