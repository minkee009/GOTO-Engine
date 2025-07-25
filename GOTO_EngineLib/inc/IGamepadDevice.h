#pragma once
#include "Vector2.h"
#include <functional>

namespace GOTOEngine
{
    enum class GamepadButton;

    enum class GamepadAxis;

    // �����е� ����/���� �̺�Ʈ �ݹ�
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class IGamepadDevice
    {
    public:
        virtual ~IGamepadDevice() = default;

        // �����е� ���� ����
        virtual bool IsConnected() const = 0;

        // �����е� ���� ������Ʈ
        virtual void Update() = 0;

        // ��ư �Է� ���� (���� �����ִ���)
        virtual bool GetButton(int buttonIndex) const = 0;
        virtual bool GetButton(GamepadButton button) const = 0;

        // ��ư �ٿ�/�� �̺�Ʈ (������ ����)
        virtual bool GetButtonDown(int buttonIndex) const = 0;
        virtual bool GetButtonDown(GamepadButton button) const = 0;
        virtual bool GetButtonUp(int buttonIndex) const = 0;
        virtual bool GetButtonUp(GamepadButton button) const = 0;

        // �Ƴ��α� �� �Է� (��ƽ, Ʈ����)
        virtual float GetAxis(int axisIndex) const = 0;
        virtual float GetAxis(GamepadAxis axis) const = 0;
        //virtual float GetAxisRaw(int axisIndex) const = 0;
        //virtual float GetAxisRaw(GamepadAxis axis) const = 0;

        // ���� �Լ���
        virtual Vector2 GetLeftStick() const = 0;   // (X, Y)
        virtual Vector2 GetRightStick() const = 0;  // (X, Y)
        virtual float GetLeftTrigger() const = 0;   // 0.0f ~ 1.0f
        virtual float GetRightTrigger() const = 0;  // 0.0f ~ 1.0f

        // ���÷��� ����
        virtual void SetConnectionCallback(GamepadConnectionCallback callback) = 0;
        virtual bool WasJustConnected() const = 0;
        virtual bool WasJustDisconnected() const = 0;
        virtual void ClearConnectionEvents() = 0;
    };
}