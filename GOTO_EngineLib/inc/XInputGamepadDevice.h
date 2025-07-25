#pragma once
#include "IGamepadDevice.h"
#include <Windows.h>
#include <Xinput.h>
#include <functional>

#pragma comment(lib, "xinput.lib")

namespace GOTOEngine
{
    // �����е� ����/���� �̺�Ʈ �ݹ�
    using GamepadConnectionCallback = std::function<void(int gamepadIndex, bool connected)>;

    class XInputGamepadDevice : public IGamepadDevice
    {
    public:
        XInputGamepadDevice(int controllerIndex);
        virtual ~XInputGamepadDevice() = default;

        // IGamepadDevice ����
        bool IsConnected() const override;
        void Update() override;

        bool GetButton(int buttonIndex) const override;
        bool GetButton(GamepadButton button) const override;

        bool GetButtonDown(int buttonIndex) const override;
        bool GetButtonDown(GamepadButton button) const override;
        bool GetButtonUp(int buttonIndex) const override;
        bool GetButtonUp(GamepadButton button) const override;

        // �Ƴ��α� �� �Է�
        float GetAxis(int axisIndex) const override;
        float GetAxis(GamepadAxis axis) const override;

        // ���� �Լ���
        Vector2 GetLeftStick() const override;
        Vector2 GetRightStick() const override;
        float GetLeftTrigger() const override;
        float GetRightTrigger() const override;

        // ���÷��� ����
        void SetConnectionCallback(GamepadConnectionCallback callback) override;
        bool WasJustConnected() const override;
        bool WasJustDisconnected() const override;
        void ClearConnectionEvents() override;

    private:
        int m_controllerIndex;
        bool m_isConnected;
        bool m_wasConnectedLastFrame;
        bool m_wasJustConnected;
        bool m_wasJustDisconnected;

        XINPUT_STATE m_currentState;
        XINPUT_STATE m_previousState;

        GamepadConnectionCallback m_connectionCallback;

        // GamepadButton�� XInput ��ư���� ����
        WORD GetXInputButton(GamepadButton button) const;
        WORD GetXInputButton(int buttonIndex) const;

        // ��ư ���� Ȯ�� ����
        bool IsButtonPressed(WORD button, const XINPUT_STATE& state) const;

        // �� �� ����ȭ (������ ó�� ����)
        float NormalizeAxis(SHORT value, SHORT deadzone) const;
        float NormalizeTrigger(BYTE value) const;

        // ���÷��� ó��
        void HandleConnectionStateChange();
        void ResetState();
    };
}