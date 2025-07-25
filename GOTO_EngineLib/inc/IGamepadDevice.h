#pragma once
#include "Vector2.h"
#include <functional>

namespace GOTOEngine
{
    enum class GamepadButton;

    enum class GamepadAxis;

    // 게임패드 연결/해제 이벤트 콜백
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class IGamepadDevice
    {
    public:
        virtual ~IGamepadDevice() = default;

        // 게임패드 연결 상태
        virtual bool IsConnected() const = 0;

        // 게임패드 상태 업데이트
        virtual void Update() = 0;

        // 버튼 입력 상태 (현재 눌려있는지)
        virtual bool GetButton(int buttonIndex) const = 0;
        virtual bool GetButton(GamepadButton button) const = 0;

        // 버튼 다운/업 이벤트 (프레임 단위)
        virtual bool GetButtonDown(int buttonIndex) const = 0;
        virtual bool GetButtonDown(GamepadButton button) const = 0;
        virtual bool GetButtonUp(int buttonIndex) const = 0;
        virtual bool GetButtonUp(GamepadButton button) const = 0;

        // 아날로그 축 입력 (스틱, 트리거)
        virtual float GetAxis(int axisIndex) const = 0;
        virtual float GetAxis(GamepadAxis axis) const = 0;
        //virtual float GetAxisRaw(int axisIndex) const = 0;
        //virtual float GetAxisRaw(GamepadAxis axis) const = 0;

        // 편의 함수들
        virtual Vector2 GetLeftStick() const = 0;   // (X, Y)
        virtual Vector2 GetRightStick() const = 0;  // (X, Y)
        virtual float GetLeftTrigger() const = 0;   // 0.0f ~ 1.0f
        virtual float GetRightTrigger() const = 0;  // 0.0f ~ 1.0f

        // 핫플러그 지원
        virtual void SetConnectionCallback(GamepadConnectionCallback callback) = 0;
        virtual bool WasJustConnected() const = 0;
        virtual bool WasJustDisconnected() const = 0;
        virtual void ClearConnectionEvents() = 0;
    };
}