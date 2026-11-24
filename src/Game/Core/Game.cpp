#include "Game.h"

void LifeExe::Core::Game::update(float dt)
{
    if (m_state == GameState::Playing)
    {
        m_totalTime += dt;
    }
}

void LifeExe::Core::Game::handleCommand(Command command)
{
    switch (command)
    {
        case LifeExe::Core::Command::Pause:
        {
            if (m_state == GameState::Playing)
            {
                m_state = GameState::Paused;
            }

            break;
        }
        case LifeExe::Core::Command::Resume:
        {
            if (m_state == GameState::Paused)
            {
                m_state = GameState::Playing;
            }

            break;
        }
        case LifeExe::Core::Command::TogglePause:
        {
            if (m_state == GameState::Playing)
            {
                m_state = GameState::Paused;
            }
            else if (m_state == GameState::Paused)
            {
                m_state = GameState::Playing;
            }

            break;
        }
        case LifeExe::Core::Command::Reset:
        {
            reset();
            break;
        }
        case LifeExe::Core::Command::Quit:
        {
            m_state = GameState::GameOver;
            break;
        }
        case LifeExe::Core::Command::None: break;
    }
}

void LifeExe::Core::Game::reset()
{
    m_state = GameState::Playing;
    m_totalTime = 0.0;
}
