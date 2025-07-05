#include "WinAPIInputSystem.h"
#include "InputManager.h" // KeyCode enum을 위해 포함

namespace GOTOEngine
{
    // Windows API의 KEY_PRESSED 정의 (InputManager.h에서 가져와서 여기에 정의)
#define KEY_PRESSED 0x8000

    WinAPIInputSystem::WinAPIInputSystem()
    {
        InitKeyCodeMap();
    }

    void WinAPIInputSystem::Startup(void* windowHandle)
    {
        m_hWnd = static_cast<HWND>(windowHandle); // 윈도우 핸들
    }

    void WinAPIInputSystem::Update()
    {
        // 마우스 좌표
        ::GetCursorPos(&m_mouseClient);
        ::ScreenToClient(m_hWnd, &m_mouseClient);

        memcpy_s(m_prevState, sizeof(m_prevState), m_currState, sizeof(m_currState));
        for (int i = 0; i < 256; i++) {
            m_currState[i] = GetAsyncKeyState(i);
        }
    }

    Vector2 WinAPIInputSystem::GetMousePos()
    {
        return Vector2{ (float)m_mouseClient.x, (float)m_mouseClient.y };
    }

    bool WinAPIInputSystem::GetKey(KeyCode keyCode)
    {
		auto nativeKeyCode = GetNativeKeyCode(keyCode);
        return (m_currState[nativeKeyCode] & KEY_PRESSED) != 0;
    }

    bool WinAPIInputSystem::GetKeyDown(KeyCode keyCode)
    {
        auto nativeKeyCode = GetNativeKeyCode(keyCode);
        return (!(m_prevState[nativeKeyCode] & KEY_PRESSED) && (m_currState[nativeKeyCode] & KEY_PRESSED));
    }

    bool WinAPIInputSystem::GetKeyUp(KeyCode keyCode)
    {
        auto nativeKeyCode = GetNativeKeyCode(keyCode);
        return ((m_prevState[nativeKeyCode] & KEY_PRESSED) && !(m_currState[nativeKeyCode] & KEY_PRESSED));
    }

    int WinAPIInputSystem::GetNativeKeyCode(KeyCode keyCode) const
    {
        auto it = m_keyCodeMap.find(keyCode);
        if (it != m_keyCodeMap.end())
        {
            return it->second;
        }
        return -1; // KeyCode가 매핑되지 않은 경우 -1 반환
    }

    void WinAPIInputSystem::InitKeyCodeMap()
    {
        // KeyCode와 Windows Virtual Key Code 매핑
        m_keyCodeMap[KeyCode::A] = 'A';
        m_keyCodeMap[KeyCode::B] = 'B';
        m_keyCodeMap[KeyCode::C] = 'C';
        m_keyCodeMap[KeyCode::D] = 'D';
        m_keyCodeMap[KeyCode::E] = 'E';
        m_keyCodeMap[KeyCode::F] = 'F';
        m_keyCodeMap[KeyCode::G] = 'G';
        m_keyCodeMap[KeyCode::H] = 'H';
        m_keyCodeMap[KeyCode::I] = 'I';
        m_keyCodeMap[KeyCode::J] = 'J';
        m_keyCodeMap[KeyCode::K] = 'K';
        m_keyCodeMap[KeyCode::L] = 'L';
        m_keyCodeMap[KeyCode::M] = 'M';
        m_keyCodeMap[KeyCode::N] = 'N';
        m_keyCodeMap[KeyCode::O] = 'O';
        m_keyCodeMap[KeyCode::P] = 'P';
        m_keyCodeMap[KeyCode::Q] = 'Q';
        m_keyCodeMap[KeyCode::R] = 'R';
        m_keyCodeMap[KeyCode::S] = 'S';
        m_keyCodeMap[KeyCode::T] = 'T';
        m_keyCodeMap[KeyCode::U] = 'U';
        m_keyCodeMap[KeyCode::V] = 'V';
        m_keyCodeMap[KeyCode::W] = 'W';
        m_keyCodeMap[KeyCode::X] = 'X';
        m_keyCodeMap[KeyCode::Y] = 'Y';
        m_keyCodeMap[KeyCode::Z] = 'Z';

        m_keyCodeMap[KeyCode::Alpha0] = '0';
        m_keyCodeMap[KeyCode::Alpha1] = '1';
        m_keyCodeMap[KeyCode::Alpha2] = '2';
        m_keyCodeMap[KeyCode::Alpha3] = '3';
        m_keyCodeMap[KeyCode::Alpha4] = '4';
        m_keyCodeMap[KeyCode::Alpha5] = '5';
        m_keyCodeMap[KeyCode::Alpha6] = '6';
        m_keyCodeMap[KeyCode::Alpha7] = '7';
        m_keyCodeMap[KeyCode::Alpha8] = '8';
        m_keyCodeMap[KeyCode::Alpha9] = '9';

        m_keyCodeMap[KeyCode::Escape] = VK_ESCAPE;
        m_keyCodeMap[KeyCode::Space] = VK_SPACE;
        m_keyCodeMap[KeyCode::Enter] = VK_RETURN;
        m_keyCodeMap[KeyCode::Tab] = VK_TAB;
        m_keyCodeMap[KeyCode::Backspace] = VK_BACK;
        m_keyCodeMap[KeyCode::Delete] = VK_DELETE;

        m_keyCodeMap[KeyCode::LeftShift] = VK_LSHIFT;
        m_keyCodeMap[KeyCode::RightShift] = VK_RSHIFT;
        m_keyCodeMap[KeyCode::LeftControl] = VK_LCONTROL;
        m_keyCodeMap[KeyCode::RightControl] = VK_RCONTROL;
        m_keyCodeMap[KeyCode::LeftAlt] = VK_LMENU; // VK_MENU는 Alt 키
        m_keyCodeMap[KeyCode::RightAlt] = VK_RMENU;

        m_keyCodeMap[KeyCode::UpArrow] = VK_UP;
        m_keyCodeMap[KeyCode::DownArrow] = VK_DOWN;
        m_keyCodeMap[KeyCode::LeftArrow] = VK_LEFT;
        m_keyCodeMap[KeyCode::RightArrow] = VK_RIGHT;

        m_keyCodeMap[KeyCode::MouseLeft] = VK_LBUTTON;
        m_keyCodeMap[KeyCode::MouseRight] = VK_RBUTTON;
        m_keyCodeMap[KeyCode::MouseMiddle] = VK_MBUTTON;
    }
}