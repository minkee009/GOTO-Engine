#pragma once
#include "Singleton.h"
#include "Vector2.h"
#include <functional>

#define MAX_GAMEPADS 4

namespace GOTOEngine
{
    // Ű����/���콺 ���� KeyCode
    enum class KeyCode
    {
        // ���ĺ�
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        // ����
        Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

        // Ư�� Ű
        Escape, Space, Enter, Tab, Backspace, Delete,
        LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
        UpArrow, DownArrow, LeftArrow, RightArrow,

        // ���콺 ��ư
        MouseLeft, MouseRight, MouseMiddle,

        // ��Ÿ
        Unknown,
    };

    // �����е� ��ư (�÷��� ������)
    enum class GamepadButton
    {
        ButtonSouth = 0,  // A / Cross
        ButtonEast,       // B / Circle  
        ButtonNorth,      // Y / Triangle
        ButtonWest,       // X / Square
        ButtonL1,         // Left Shoulder
        ButtonR1,         // Right Shoulder
        ButtonStart,      // Start/Options/Menu
        ButtonSelect,     // Back/Share/View
        DPadUp,           // D-Pad ��
        DPadDown,         // D-Pad �Ʒ�
        DPadLeft,         // D-Pad ����
        DPadRight,        // D-Pad ������
        Count
    };

    // �����е� �� (�÷��� ������)
    enum class GamepadAxis
    {
        LeftStickX = 0,
        LeftStickY,
        RightStickX,
        RightStickY,
        LeftTrigger,
        RightTrigger,
        DPadX,          // D-Pad ������ (-1: ����, 0: �߸�, +1: ������)
        DPadY,          // D-Pad ������ (-1: �Ʒ�, 0: �߸�, +1: ��)
        Count
    };

    // �����е� ����/���� �̺�Ʈ �ݹ�
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class IInputSystem;
    class InputManager : public Singleton<InputManager>
    {
    public:
        InputManager() = default;
        ~InputManager() = default;

        void Startup(void* windowHandle);
        void Shutdown();
        void Update();

        // === Ű���� �� ���콺 �Է� ===
        Vector2 GetMousePos();
        bool GetKey(KeyCode keyCode);
        bool GetKeyDown(KeyCode keyCode);
        bool GetKeyUp(KeyCode keyCode);

        // === �����е� �Է� ===
        // ��ư �Է�
        bool GetGamepadButton(int gamepadIndex, GamepadButton button);
        bool GetGamepadButtonDown(int gamepadIndex, GamepadButton button);
        bool GetGamepadButtonUp(int gamepadIndex, GamepadButton button);

        // �Ƴ��α� �� �Է�
        float GetGamepadAxis(int gamepadIndex, GamepadAxis axis);
        float GetGamepadAxisRaw(int gamepadIdex, GamepadAxis axis);
        Vector2 GetLeftStick(int gamepadIndex);   // ���� ��ƽ (X, Y)
        Vector2 GetRightStick(int gamepadIndex);  // ������ ��ƽ (X, Y)
        float GetLeftTrigger(int gamepadIndex);   // ���� Ʈ���� (0.0f ~ 1.0f)
        float GetRightTrigger(int gamepadIndex);  // ������ Ʈ���� (0.0f ~ 1.0f)

        // D-Pad ���� �Լ���
        Vector2 GetDPad(int gamepadIndex);           // D-Pad �� (X, Y)
        float GetDPadX(int gamepadIndex);            // D-Pad ������ (-1, 0, +1)
        float GetDPadY(int gamepadIndex);            // D-Pad ������ (-1, 0, +1)

        // === �����е� ���� ���� ===
        bool IsGamepadConnected(int gamepadIndex);
        int GetConnectedGamepadCount();
        int GetFirstConnectedGamepad();

        // === ���÷��� ���� ===
        void SetGamepadConnectionCallback(GamepadConnectionCallback callback);
        bool WasGamepadJustConnected(int gamepadIndex);
        bool WasGamepadJustDisconnected(int gamepadIndex);
        void ClearGamepadConnectionEvents();

    private:
        IInputSystem* m_inputSystem = nullptr;
        GamepadConnectionCallback m_connectionCallback;
    };
}

// === Ű����/���콺 �Է� ��ũ�� ===
#define INPUT_GET_KEY GOTOEngine::InputManager::Get()->GetKey
#define INPUT_GET_KEYDOWN GOTOEngine::InputManager::Get()->GetKeyDown
#define INPUT_GET_KEYUP GOTOEngine::InputManager::Get()->GetKeyUp
#define INPUT_GET_MOUSEPOS GOTOEngine::InputManager::Get()->GetMousePos

// === �����е� �Է� ��ũ�� ===
#define INPUT_GET_GAMEPAD_BUTTON(index, button) GOTOEngine::InputManager::Get()->GetGamepadButton(index, button)
#define INPUT_GET_GAMEPAD_BUTTONDOWN(index, button) GOTOEngine::InputManager::Get()->GetGamepadButtonDown(index, button)
#define INPUT_GET_GAMEPAD_BUTTONUP(index, button) GOTOEngine::InputManager::Get()->GetGamepadButtonUp(index, button)
#define INPUT_GET_GAMEPAD_AXIS(index, axis) GOTOEngine::InputManager::Get()->GetGamepadAxis(index, axis)
#define INPUT_GET_GAMEPAD_AXISRAW(index, axis) GOTOEngine::InputManager::Get()->GetGamepadAxisRaw(index, axis)
#define INPUT_GAMEPAD_IS_CONNECTED(index) GOTOEngine::InputManager::Get()->IsGamepadConnected(index)
#define INPUT_GET_LEFTSTICK(index) GOTOEngine::InputManager::Get()->GetLeftStick(index)
#define INPUT_GET_RIGHTSTICK(index) GOTOEngine::InputManager::Get()->GetRightStick(index)
#define INPUT_GET_LEFTTRIGGER(index) GOTOEngine::InputManager::Get()->GetLeftTrigger(index)
#define INPUT_GET_RIGHTTRIGGER(index) GOTOEngine::InputManager::Get()->GetRightTrigger(index)

// === D-Pad �Է� ��ũ�� ===
#define INPUT_GET_DPAD(index) GOTOEngine::InputManager::Get()->GetDPad(index)
#define INPUT_GET_DPAD_X(index) GOTOEngine::InputManager::Get()->GetDPadX(index)
#define INPUT_GET_DPAD_Y(index) GOTOEngine::InputManager::Get()->GetDPadY(index)

// === ���÷��� ��ũ�� ===
#define INPUT_GAMEPAD_JUST_CONNECTED(index) GOTOEngine::InputManager::Get()->WasGamepadJustConnected(index)
#define INPUT_GAMEPAD_JUST_DISCONNECTED(index) GOTOEngine::InputManager::Get()->WasGamepadJustDisconnected(index)