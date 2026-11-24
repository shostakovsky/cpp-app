#include <gtest/gtest.h>

#include "Game.h"

namespace LifeExe::Core
{
TEST(GameTest, InitialState)
{
    const Game game;

    EXPECT_EQ(game.state(), GameState::Playing);
    EXPECT_FLOAT_EQ(game.totalTime(), 0.0f);
}

TEST(GameTest, AccumulatesTotalTimeWhilePlaying)
{
    Game game;

    game.update(0.5f);
    game.update(0.25f);

    EXPECT_FLOAT_EQ(game.totalTime(), 0.75f);
}

TEST(GameTest, SnapshotCopiesCoreState)
{
    Game game;
    game.update(0.5f);

    const auto snapshot = game.snapshot();

    EXPECT_EQ(snapshot.state, GameState::Playing);
    EXPECT_FLOAT_EQ(snapshot.totalTime, 0.5f);
}

TEST(GameTest, GameStateToString)
{
    EXPECT_EQ(toString(GameState::Playing), "Playing");
    EXPECT_EQ(toString(GameState::Paused), "Paused");
    EXPECT_EQ(toString(GameState::GameOver), "GameOver");
}
}  // namespace LifeExe::Core
