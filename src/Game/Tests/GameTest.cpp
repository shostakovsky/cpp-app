#include <gtest/gtest.h>

#include "Game.h"

namespace LifeExe::Core
{
TEST(GameTest, InitialState)
{
    const Game game;

    EXPECT_EQ(game.state(), GameState::Playing);
    EXPECT_DOUBLE_EQ(game.totalTime(), 0.0);
}

TEST(GameTest, AccumulatesTotalTimeWhilePlaying)
{
    Game game;

    game.update(0.5f);
    game.update(0.25f);

    EXPECT_DOUBLE_EQ(game.totalTime(), 0.75);
}

TEST(GameTest, SnapshotCopiesCoreState)
{
    Game game;
    game.update(0.5f);

    const auto snapshot = game.snapshot();

    EXPECT_EQ(snapshot.state, GameState::Playing);
    EXPECT_DOUBLE_EQ(snapshot.totalTime, 0.5);
}

TEST(GameTest, GameStateToString)
{
    EXPECT_EQ(toString(GameState::Playing), "Playing");
    EXPECT_EQ(toString(GameState::Paused), "Paused");
    EXPECT_EQ(toString(GameState::GameOver), "GameOver");
}

TEST(GameTest, HandlesPauseAndResumeCommands)
{
    Game game;

    game.handleCommand(Command::Pause);
    EXPECT_EQ(game.state(), GameState::Paused);

    game.update(1.0f);
    EXPECT_DOUBLE_EQ(game.totalTime(), 0.0);

    game.handleCommand(Command::Resume);
    EXPECT_EQ(game.state(), GameState::Playing);

    game.update(0.25f);
    EXPECT_DOUBLE_EQ(game.totalTime(), 0.25);
}

TEST(GameTest, HandlesTogglePauseCommand)
{
    Game game;

    game.handleCommand(Command::TogglePause);
    EXPECT_EQ(game.state(), GameState::Paused);

    game.handleCommand(Command::TogglePause);
    EXPECT_EQ(game.state(), GameState::Playing);

    game.handleCommand(Command::Quit);
    game.handleCommand(Command::TogglePause);
    EXPECT_EQ(game.state(), GameState::GameOver);
}

TEST(GameTest, HandlesResetCommand)
{
    Game game;
    game.update(0.5f);
    game.handleCommand(Command::Pause);

    game.handleCommand(Command::Reset);

    EXPECT_EQ(game.state(), GameState::Playing);
    EXPECT_DOUBLE_EQ(game.totalTime(), 0.0);
}

TEST(GameTest, HandlesQuitCommand)
{
    Game game;

    game.handleCommand(Command::Quit);

    EXPECT_EQ(game.state(), GameState::GameOver);
}

TEST(GameTest, CommandToString)
{
    EXPECT_EQ(toString(Command::None), "None");
    EXPECT_EQ(toString(Command::Pause), "Pause");
    EXPECT_EQ(toString(Command::Resume), "Resume");
    EXPECT_EQ(toString(Command::TogglePause), "TogglePause");
    EXPECT_EQ(toString(Command::Reset), "Reset");
    EXPECT_EQ(toString(Command::Quit), "Quit");
}
}  // namespace LifeExe::Core
