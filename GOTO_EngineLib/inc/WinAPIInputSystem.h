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
        HWND m_hWnd; // 윈도우 핸들
        POINT m_mouseClient; // 마우스 좌표
        SHORT m_prevState[256] = { 0 };
        SHORT m_currState[256] = { 0 };

		std::map<KeyCode, int> m_keyCodeMap; // KeyCode와 Windows VKey 매핑

        void InitKeyCodeMap(); // KeyCode를 Windows VKey로 매핑
    };
}