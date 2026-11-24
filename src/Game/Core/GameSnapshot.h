#pragma once

#include "Types.h"

namespace LifeExe::Core
{
/// Render-time copy of the current game state.
struct GameSnapshot
{
    /// Current lifecycle state of the game.
    GameState state{GameState::Playing};

    /// Total elapsed game time.
    float totalTime{0.0f};
};
}  // namespace LifeExe::Core
