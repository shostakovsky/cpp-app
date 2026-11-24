#include "Game.h"

void LifeExe::Core::Game::update(float dt)
{
    if (m_state == GameState::Playing)
    {
        m_totalTime += dt;
    }
}
