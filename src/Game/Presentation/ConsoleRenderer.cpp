#include "ConsoleRenderer.h"

#include <print>

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void LifeExe::Presentation::ConsoleRenderer::render(const Core::GameSnapshot& snapshot) const
{
    std::println("[Frame Render] State: {}, Time: {:.2f}s", Core::toString(snapshot.state), snapshot.totalTime);
}
