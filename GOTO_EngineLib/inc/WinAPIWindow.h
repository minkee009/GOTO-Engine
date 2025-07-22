#pragma once
#include <IWindow.h>
#include <Windows.h>
#include <string>
#include "Delegate.h"

namespace GOTOEngine
{
    class WinAPIWindow : public IWindow {
    public:
        bool Create(int width, int height, const wchar_t* title) override;
        void* GetNativeHandle() const override { return m_hwnd; }
        void PollEvents() override;
        bool ShouldClose() const override { return m_shouldClose; }
        void SetOnChangedWindowSize(std::function<void(int,int)> callback) override { m_onChangedWindowSize += callback; }

        int GetWidth() const override;
        int GetHeight() const override;

        bool IsFullScreen() const override { return m_isFullScreen; }
        void ToggleFullScreen() override;

        virtual void HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual ~WinAPIWindow() { m_onChangedWindowSize.Clear(); };
    protected:
        std::wstring m_className = L"WinAPIWindowClass";
        std::wstring m_title = L"WinAPIWindow";
        HWND m_hwnd = nullptr;
        int m_width = 1024;
        int m_height = 768;
        bool m_shouldClose = false;
        Delegate<void, int, int> m_onChangedWindowSize;

        bool m_isFullScreen = false;
        RECT m_savedWindowRect = {}; // 창 모드일 때 위치/크기 저장
        DWORD m_savedStyle = 0;      // 창 모드일 때 스타일 저장
        DWORD m_savedExStyle = 0;    // 창 모드일 때 확장 스타일 저장
    };
}