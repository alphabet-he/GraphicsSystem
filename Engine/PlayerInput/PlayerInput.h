#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include "Engine/UserInput/UserInput.h"
#include <Engine/Math/sVector.h>
#include <unordered_map>
#include <External/SDL2/include/SDL.h>

namespace eae6320
{
    class PlayerInput
    {
    public:
        enum class Action
        {
            ObjectMoveForward,
            ObjectMoveBackward,
            ObjectMoveLeft,
            ObjectMoveRight,
            CameraMoveForward,
            CameraMoveBackward,
            CameraMoveLeft,
            CameraMoveRight,
            SwitchMesh,
            QuitGame,
        };

        enum class KeyCode : uint8_t
        {
            Left = 0x25,
            Up = 0x26,
            Right = 0x27,
            Down = 0x28,

            Space = 0x20,
            Escape = 0x1B,
            Shift = 0x10,
            Control = 0x11,
            Alt = 0x12,

            Tab = 0x09,
            CapsLock = 0x14,
            BackSpace = 0x08,
            Enter = 0x0D,
            Delete = 0x2E,

            PageUp = 0x21,
            PageDown = 0x22,
            End = 0x23,
            Home = 0x24,

            F1 = 0x70,
            F2 = 0x71,
            F3 = 0x72,
            F4 = 0x73,
            F5 = 0x74,
            F6 = 0x75,
            F7 = 0x76,
            F8 = 0x77,
            F9 = 0x78,
            F10 = 0x79,
            F11 = 0x7A,
            F12 = 0x7B,

            W = 0x57,
            A = 0x41,
            S = 0x53,
            D = 0x44
        };

        struct InputState
        {
            Math::sVector objectMoveDirection;
            Math::sVector cameraMoveDirection;
            bool switchMesh = false;
            bool quitGame = false;
        };

        PlayerInput();
        ~PlayerInput(); // Destructor to close the game controller

        void BindKey(Action action, KeyCode keyCode);
        void Update(InputState& o_inputState);

    private:
        std::unordered_map<Action, KeyCode> m_keyBindings;
        SDL_Joystick* m_pGameController = nullptr; // Pointer to game controller

        void UpdateObjectMovement(Math::sVector& o_movementDirection);
        void UpdateCameraMovement(Math::sVector& o_movementDirection);
        void UpdateSwitchMesh(bool& o_switchMesh);
        void UpdateQuitGame(bool& o_quitGame);
    };
}

#endif // PLAYERINPUT_H
