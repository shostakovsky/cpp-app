#pragma once

#include "GameSnapshot.h"

namespace LifeExe::Presentation
{
/// Renders game snapshots to the console.
class ConsoleRenderer
{
public:
    /// Prints the supplied game snapshot.
    /// @param snapshot State and timing data to render.
    void render(const Core::GameSnapshot& snapshot) const;
};
}  // namespace LifeExe::Presentation