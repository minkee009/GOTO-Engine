#pragma once
#include <Windows.h>
#include <array>
#include <map>
#include <functional>
#include "IInputSystem.h"
#include "IGamepadDevice.h"

namespace GOTOEngine
{
    class XInputGamepadDevice; // ���� ����

    // �����е� ����/���� �̺�Ʈ �ݹ�
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class WinAPIInputSystem : public IInputSystem
    {
    public:
        WinAPIInputSystem();
        virtual ~WinAPIInputSystem();

        void Startup(void* windowHandle) override;
        void Update() override;
        Vector2 GetMousePos() override;
        bool GetKey(KeyCode keyCode) override;
        bool GetKeyDown(KeyCode keyCode) override;
        bool GetKeyUp(KeyCode keyCode) override;
        int GetNativeKeyCode(KeyCode keyCode) const override;

        IGamepadDevice* GetGamepad(int index) override;

        // ���÷��� ����
        void SetGamepadConnectionCallback(GamepadConnectionCallback callback) override;

    private:
        HWND m_hWnd; // ������ �ڵ�
        POINT m_mouseClient; // ���콺 ��ǥ
        SHORT m_prevState[256] = { 0 };
        SHORT m_currState[256] = { 0 };

        RECT m_clientRect;

        std::map<KeyCode, int> m_keyCodeMap; // KeyCode�� Windows VKey ����

        void InitKeyCodeMap(); // KeyCode�� Windows VKey�� ����

        // �����е� ���� (�ִ� 4��)
        std::array<XInputGamepadDevice*, 4> m_gamepads;
        void InitGamepads(); // �����е� �ʱ�ȭ
        void CleanupGamepads(); // �����е� ����
    };
}