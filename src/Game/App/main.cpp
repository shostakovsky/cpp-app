#include "Game.h"
#include "Types.h"
#include "ConsoleRenderer.h"
#include "Command.h"
#include "Config.h"
#include "Timer.h"

#include <print>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace
{
std::vector<LifeExe::Core::Command> processInput(int frame)
{
    if (frame == 3)
    {
        return {LifeExe::Core::Command::Pause};
    }

    if (frame == 6)
    {
        return {LifeExe::Core::Command::Resume};
    }

    if (frame == 9)
    {
        return {LifeExe::Core::Command::Quit};
    }

    return {};
}
}  // namespace

int main()
{
    try
    {
        std::ifstream configFile{"config.json"};
        const LifeExe::GameConfig config{configFile.is_open()
                                             ? LifeExe::parseConfig(configFile)
                                             : LifeExe::GameConfig{.title = "LifeExe Game Framework", .width = 800, .height = 600}};
        std::println("Starting {} ({}x{})...", config.title, config.width, config.height);

        LifeExe::Core::Game game;
        LifeExe::App::Timer timer;
        const LifeExe::Presentation::ConsoleRenderer renderer;

        std::println("Starting Game...");

        constexpr float fixedDt{1.0f / 60.0f};
        constexpr float maxFrameTime{0.25f};
        float accumulator{0.0f};
        int simulatedFrames{0};

        while (game.state() != LifeExe::Core::GameState::GameOver && simulatedFrames < 10)
        {
            const float frameTime{std::min(timer.tick(), maxFrameTime)};
            accumulator += frameTime;

            for (const LifeExe::Core::Command command : processInput(simulatedFrames))
            {
                game.handleCommand(command);
            }

            while (accumulator >= fixedDt)
            {
                game.update(fixedDt);
                accumulator -= fixedDt;
            }

            renderer.render(game.snapshot());
            simulatedFrames++;
            std::this_thread::sleep_for(std::chrono::milliseconds{16});
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