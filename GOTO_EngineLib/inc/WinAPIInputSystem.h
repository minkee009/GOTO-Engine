#pragma once
#include "IInputSystem.h"
#include <Windows.h>

namespace GOTOEngine
{
    class WinAPIInputSystem : public IInputSystem
    {
    public:
        WinAPIInputSystem();
        virtual ~WinAPIInputSystem() = default;

        void Startup(void* windowHandle) override;
        void Update() override;
        Vector2 GetMousePos() override;
        bool GetKey(KeyCode keyCode) override;
        bool GetKeyDown(KeyCode keyCode) override;
        bool GetKeyUp(KeyCode keyCode) override;
        int GetNativeKeyCode(KeyCode keyCode) const override;

    private:
        HWND m_hWnd; // ������ �ڵ�
        POINT m_mouseClient; // ���콺 ��ǥ
        SHORT m_prevState[256] = { 0 };
        SHORT m_currState[256] = { 0 };

		std::map<KeyCode, int> m_keyCodeMap; // KeyCode�� Windows VKey ����

        void InitKeyCodeMap(); // KeyCode�� Windows VKey�� ����
    };
}