#include "InputManager.h"
#include "IInputSystem.h"
#include "IGamepadDevice.h"
#ifdef _OS_WINDOWS
#include "WinAPIInputSystem.h"
#include "XInputGamepadDevice.h"  // XInputGamepadDevice 캐스팅을 위해 필요
#endif

using namespace GOTOEngine;

void InputManager::Startup(void* windowHandle)
{
#ifdef _OS_WINDOWS
    m_inputSystem = new WinAPIInputSystem();
    m_inputSystem->Startup(windowHandle);
#endif
}

void InputManager::Shutdown()
{
    if (m_inputSystem)
    {
        delete m_inputSystem;
        m_inputSystem = nullptr;
    }
}

void InputManager::Update()
{
    if (m_inputSystem)
        m_inputSystem->Update();
}

Vector2 InputManager::GetMousePos()
{
    if (m_inputSystem)
        return m_inputSystem->GetMousePos();

    return Vector2::Zero();
}

bool InputManager::GetKey(KeyCode keyCode)
{
    if (m_inputSystem)
        return m_inputSystem->GetKey(keyCode);

    return false;
}

bool InputManager::GetKeyDown(KeyCode keyCode)
{
    if (m_inputSystem)
        return m_inputSystem->GetKeyDown(keyCode);

    return false;
}

bool InputManager::GetKeyUp(KeyCode keyCode)
{
    if (m_inputSystem)
        return m_inputSystem->GetKeyUp(keyCode);

    return false;
}

bool InputManager::GetGamepadButton(int gamepadIndex, GamepadButton button)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetButton(static_cast<int>(button));

    return false;
}

bool InputManager::GetGamepadButtonDown(int gamepadIndex, GamepadButton button)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetButtonDown(static_cast<int>(button));

    return false;
}

bool InputManager::GetGamepadButtonUp(int gamepadIndex, GamepadButton button)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetButtonUp(static_cast<int>(button));

    return false;
}

float InputManager::GetGamepadAxis(int gamepadIndex, GamepadAxis axis)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return 0.0f;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetAxis(static_cast<int>(axis));

    return 0.0f;
}

Vector2 InputManager::GetLeftStick(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return Vector2::Zero();

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetLeftStick();

    return Vector2::Zero();
}

Vector2 InputManager::GetRightStick(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return Vector2::Zero();

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetRightStick();

    return Vector2::Zero();
}

float InputManager::GetLeftTrigger(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return 0.0f;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetLeftTrigger();

    return 0.0f;
}

float InputManager::GetRightTrigger(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return 0.0f;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad && pad->IsConnected())
        return pad->GetRightTrigger();

    return 0.0f;
}

bool InputManager::IsGamepadConnected(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    return pad && pad->IsConnected();
}

int InputManager::GetConnectedGamepadCount()
{
    if (!m_inputSystem)
        return 0;

    int count = 0;
    for (int i = 0; i < MAX_GAMEPADS; ++i)
    {
        if (IsGamepadConnected(i))
            count++;
    }
    return count;
}

int InputManager::GetFirstConnectedGamepad()
{
    if (!m_inputSystem)
        return -1;

    for (int i = 0; i < MAX_GAMEPADS; ++i)
    {
        if (IsGamepadConnected(i))
            return i;
    }
    return -1;
}

void InputManager::SetGamepadConnectionCallback(GamepadConnectionCallback callback)
{
    m_connectionCallback = callback;

    if (m_inputSystem)
    {
        // IInputSystem을 통해 콜백 설정
        m_inputSystem->SetGamepadConnectionCallback(callback);
    }
}

bool InputManager::WasGamepadJustConnected(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad)
    {
        return pad->WasJustConnected();
    }
    return false;
}

bool InputManager::WasGamepadJustDisconnected(int gamepadIndex)
{
    if (!m_inputSystem || gamepadIndex < 0 || gamepadIndex >= MAX_GAMEPADS)
        return false;

    IGamepadDevice* pad = m_inputSystem->GetGamepad(gamepadIndex);
    if (pad)
    {
        return pad->WasJustDisconnected();
    }
    return false;
}

void InputManager::ClearGamepadConnectionEvents()
{
    if (!m_inputSystem)
        return;

    for (int i = 0; i < MAX_GAMEPADS; ++i)
    {
        IGamepadDevice* pad = m_inputSystem->GetGamepad(i);
        if (pad)
        {
            pad->ClearConnectionEvents();
        }
    }
}