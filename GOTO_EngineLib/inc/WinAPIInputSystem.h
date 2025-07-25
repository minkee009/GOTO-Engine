#pragma once
#include <Windows.h>
#include <array>
#include <map>
#include <functional>
#include "IInputSystem.h"
#include "IGamepadDevice.h"

namespace GOTOEngine
{
    class XInputGamepadDevice; // 전방 선언

    // 게임패드 연결/해제 이벤트 콜백
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

        // 핫플러그 지원
        void SetGamepadConnectionCallback(GamepadConnectionCallback callback) override;

    private:
        HWND m_hWnd; // 윈도우 핸들
        POINT m_mouseClient; // 마우스 좌표
        SHORT m_prevState[256] = { 0 };
        SHORT m_currState[256] = { 0 };

        RECT m_clientRect;

        std::map<KeyCode, int> m_keyCodeMap; // KeyCode와 Windows VKey 매핑

        void InitKeyCodeMap(); // KeyCode를 Windows VKey로 매핑

        // 게임패드 관리 (최대 4개)
        std::array<XInputGamepadDevice*, 4> m_gamepads;
        void InitGamepads(); // 게임패드 초기화
        void CleanupGamepads(); // 게임패드 정리
    };
}