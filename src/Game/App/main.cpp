#include "Game.h"
#include "Types.h"
#include "ConsoleRenderer.h"

#include <print>
#include <exception>
#include <cstdio>
#include <cstdlib>

int main()
{
    try
    {
        LifeExe::Core::Game game;
        const LifeExe::Presentation::ConsoleRenderer renderer;

        std::println("Starting Game...");

        constexpr int frames{5};
        constexpr float dt{0.016f};
        for (int frame{0}; frame < frames; frame++)
        {
            if (game.state() != LifeExe::Core::GameState::Playing)
            {
                break;
            }

            game.update(dt);
            renderer.render(game.snapshot());
        }

        std::println("Game finished successfully.");
    }
    catch (const std::exception& e)
    {
        std::fputs(e.what(), stderr);

        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::fputs("Unknown error \n", stderr);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}