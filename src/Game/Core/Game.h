#pragma once

#include "GameSnapshot.h"

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
    float totalTime() const { return m_totalTime; }

    /// Returns a copy of the game state for rendering.
    GameSnapshot snapshot() const { return GameSnapshot{.state = m_state, .totalTime = m_totalTime}; }

private:
    GameState m_state{GameState::Playing};
    float m_totalTime{0.0f};
};
}  // namespace LifeExe::Core
