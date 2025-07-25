#pragma once
#include "Vector2.h" 
#include <map>
#include <functional>

namespace GOTOEngine
{
    enum class KeyCode;

    class IGamepadDevice;

    // 게임패드 연결/해제 이벤트 콜백
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class IInputSystem
    {
    public:
        virtual ~IInputSystem() = default;

        // OS별 초기화 함수 (예: 윈도우 핸들 전달)
        virtual void Startup(void* windowHandle) = 0;
        virtual void Update() = 0;
        virtual Vector2 GetMousePos() = 0;
        virtual bool GetKey(KeyCode keyCode) = 0; // OS별 native key code 사용
        virtual bool GetKeyDown(KeyCode keyCode) = 0;
        virtual bool GetKeyUp(KeyCode keyCode) = 0;

        // KeyCode를 네이티브 코드로 변환하는 순수 가상 함수 추가
        virtual int GetNativeKeyCode(KeyCode keyCode) const = 0;

        virtual IGamepadDevice* GetGamepad(int index) = 0;

        // 핫플러그 지원
        virtual void SetGamepadConnectionCallback(GamepadConnectionCallback callback) = 0;
    };
}