#include "PlayerInput.h"
#include "Engine/UserInput/UserInput.h"
#include <SDL.h>

namespace eae6320
{
    PlayerInput::PlayerInput()
    {
        // Initialize the key bindings
        BindKey(Action::ObjectMoveForward, KeyCode::W);
        BindKey(Action::ObjectMoveBackward, KeyCode::S);
        BindKey(Action::ObjectMoveLeft, KeyCode::A);
        BindKey(Action::ObjectMoveRight, KeyCode::D);
        BindKey(Action::CameraMoveForward, KeyCode::Up);
        BindKey(Action::CameraMoveBackward, KeyCode::Down);
        BindKey(Action::CameraMoveLeft, KeyCode::Left);
        BindKey(Action::CameraMoveRight, KeyCode::Right);
        BindKey(Action::SwitchMesh, KeyCode::Space);
        BindKey(Action::QuitGame, KeyCode::Escape);

        // Initialize SDL Joystick
        if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
            SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        }
        else {
            if (SDL_NumJoysticks() > 0) {
                m_pGameController = SDL_JoystickOpen(0);
                if (!m_pGameController) {
                    SDL_Log("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
                }
            }
        }
    }

    PlayerInput::~PlayerInput()
    {
        if (m_pGameController) {
            SDL_JoystickClose(m_pGameController);
            m_pGameController = nullptr;
        }
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }

    void PlayerInput::BindKey(Action action, KeyCode keyCode)
    {
        m_keyBindings[action] = keyCode;
    }

    void PlayerInput::Update(InputState& o_inputState)
    {
        o_inputState.objectMoveDirection = Math::sVector(0.0f, 0.0f, 0.0f);
        o_inputState.cameraMoveDirection = Math::sVector(0.0f, 0.0f, 0.0f);
        o_inputState.switchMesh = false;
        o_inputState.quitGame = false;

        UpdateObjectMovement(o_inputState.objectMoveDirection);
        UpdateCameraMovement(o_inputState.cameraMoveDirection);
        UpdateSwitchMesh(o_inputState.switchMesh);
        UpdateQuitGame(o_inputState.quitGame);
    }

    void PlayerInput::UpdateObjectMovement(Math::sVector& o_movementDirection)
    {
        // Keyboard input
        for (const auto& pair : m_keyBindings)
        {
            Action action = pair.first;
            KeyCode keyCode = pair.second;

            if (UserInput::IsKeyPressed(static_cast<uint8_t>(keyCode)))
            {
                switch (action)
                {
                case Action::ObjectMoveForward:
                    o_movementDirection.z -= 1.0f;
                    break;
                case Action::ObjectMoveBackward:
                    o_movementDirection.z += 1.0f;
                    break;
                case Action::ObjectMoveLeft:
                    o_movementDirection.x -= 1.0f;
                    break;
                case Action::ObjectMoveRight:
                    o_movementDirection.x += 1.0f;
                    break;
                }
            }
        }

        // Gamepad input
        if (m_pGameController)
        {
            const int deadZone = 8000;  // Dead zone for joystick movement
            int xAxis = SDL_JoystickGetAxis(m_pGameController, 0);
            int yAxis = SDL_JoystickGetAxis(m_pGameController, 1);

            if (yAxis < -deadZone)
                o_movementDirection.z -= 1.0f;
            else if (yAxis > deadZone)
                o_movementDirection.z += 1.0f;

            if (xAxis < -deadZone)
                o_movementDirection.x -= 1.0f;
            else if (xAxis > deadZone)
                o_movementDirection.x += 1.0f;
        }
    }

    void PlayerInput::UpdateCameraMovement(Math::sVector& o_movementDirection)
    {
        if (m_pGameController)
        {
            const int deadZone = 8000;  // Dead zone for joystick movement

            // Right joystick controls camera movement
            int xAxis = SDL_JoystickGetAxis(m_pGameController, 2);  // Right X axis
            int yAxis = SDL_JoystickGetAxis(m_pGameController, 3);  // Right Y axis

            if (yAxis < -deadZone)
                o_movementDirection.z -= 1.0f;
            else if (yAxis > deadZone)
                o_movementDirection.z += 1.0f;

            if (xAxis < -deadZone)
                o_movementDirection.x -= 1.0f;
            else if (xAxis > deadZone)
                o_movementDirection.x += 1.0f;
        }

        // Keyboard input
        for (const auto& pair : m_keyBindings)
        {
            Action action = pair.first;
            KeyCode keyCode = pair.second;

            if (UserInput::IsKeyPressed(static_cast<uint8_t>(keyCode)))
            {
                switch (action)
                {
                case Action::CameraMoveForward:
                    o_movementDirection.z -= 1.0f;
                    break;
                case Action::CameraMoveBackward:
                    o_movementDirection.z += 1.0f;
                    break;
                case Action::CameraMoveLeft:
                    o_movementDirection.x -= 1.0f;
                    break;
                case Action::CameraMoveRight:
                    o_movementDirection.x += 1.0f;
                    break;
                }
            }
        }
    }

    void PlayerInput::UpdateSwitchMesh(bool& o_switchMesh)
    {
        o_switchMesh = UserInput::IsKeyPressed(static_cast<uint8_t>(m_keyBindings[Action::SwitchMesh]));
        if (m_pGameController)
        {
            o_switchMesh = o_switchMesh || SDL_JoystickGetButton(m_pGameController, SDL_CONTROLLER_BUTTON_A);
        }
    }

    void PlayerInput::UpdateQuitGame(bool& o_quitGame)
    {
        o_quitGame = UserInput::IsKeyPressed(static_cast<uint8_t>(m_keyBindings[Action::QuitGame]));
        if (m_pGameController)
        {
            o_quitGame = o_quitGame || SDL_JoystickGetButton(m_pGameController, SDL_CONTROLLER_BUTTON_B);
        }
    }
}