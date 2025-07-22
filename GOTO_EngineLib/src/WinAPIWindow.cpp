#include "WinAPIWindow.h"

using namespace GOTOEngine;

// Windows ���ν��� �ݹ� �Լ�
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WinAPIWindow* pThis = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<WinAPIWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<WinAPIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        pThis->HandleMessage(uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


bool WinAPIWindow::Create(int width, int height, const wchar_t* title)
{
    m_title = title;

	m_width = width;
	m_height = height;

    // ������ Ŭ���� ���
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = m_className.c_str();

    if (!RegisterClassEx(&wc))
    {
        return false;
    }

    // ������ ����
    RECT rect = { 0, 0, m_width, m_height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int adjustedWidth = rect.right - rect.left;
    int adjustedHeight = rect.bottom - rect.top;

    m_hwnd = CreateWindowEx(
        0,
        m_className.c_str(),
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        adjustedWidth, adjustedHeight,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        this
    );

    if (!m_hwnd)
    {
        return false;
    }

    // ������ ǥ��
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    return true;
}

void WinAPIWindow::PollEvents()
{
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_shouldClose = true;
        } 

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// WinAPIWindow.cpp�� �߰��� ����
void WinAPIWindow::ToggleFullScreen()
{
    if (!m_hwnd)
        return;

    if (m_isFullScreen)
    {
        // ��üȭ�� -> â ���� ��ȯ
        SetWindowPos(m_hwnd, HWND_NOTOPMOST,
            m_savedWindowRect.left,
            m_savedWindowRect.top,
            m_savedWindowRect.right - m_savedWindowRect.left,
            m_savedWindowRect.bottom - m_savedWindowRect.top,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(m_hwnd, SW_NORMAL);

        // â ��� Ŭ���̾�Ʈ ���� ũ��� ������Ʈ
        RECT clientRect;
        GetClientRect(m_hwnd, &clientRect);
        m_width = clientRect.right - clientRect.left;
        m_height = clientRect.bottom - clientRect.top;

        m_isFullScreen = false;
    }
    else
    {
        // â ��� -> ��üȭ������ ��ȯ
        // ���� â ��ġ/ũ�� ����
        GetWindowRect(m_hwnd, &m_savedWindowRect);

        // ����� ���� ��������
        HMONITOR hMonitor = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = {};
        mi.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hMonitor, &mi);

        // Ÿ��Ʋ�� ���� ���
        RECT windowRect, clientRect;
        GetWindowRect(m_hwnd, &windowRect);
        GetClientRect(m_hwnd, &clientRect);
        int titleBarHeight = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

        // â�� ����� �������� �̵� (Ÿ��Ʋ�ٰ� ����������)
        // Ŭ���̾�Ʈ ������ ��Ȯ�� ����� ��ü�� ������ ���
        int windowX = mi.rcMonitor.left;
        int windowY = mi.rcMonitor.top - titleBarHeight;
        int windowWidth = mi.rcMonitor.right - mi.rcMonitor.left;
        int windowHeight = (mi.rcMonitor.bottom - mi.rcMonitor.top) + titleBarHeight;

        SetWindowPos(m_hwnd, HWND_TOPMOST,
            windowX, windowY,
            windowWidth, windowHeight,
            SWP_FRAMECHANGED | SWP_NOACTIVATE);

        ShowWindow(m_hwnd, SW_NORMAL);

        // Ŭ���̾�Ʈ ���� ũ��� ��� ���� ������Ʈ (����� ��ü ũ��)
        m_width = mi.rcMonitor.right - mi.rcMonitor.left;
        m_height = mi.rcMonitor.bottom - mi.rcMonitor.top;

        m_isFullScreen = true;
    }

    m_onChangedWindowSize(m_width, m_height);
}

void WinAPIWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        m_shouldClose = true;
        DestroyWindow(m_hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        m_width = LOWORD(lParam);
        m_height = HIWORD(lParam);
        m_onChangedWindowSize(m_width, m_height);
        break;
    }
}

int WinAPIWindow::GetWidth() const
{
    return m_width;
}

int WinAPIWindow::GetHeight() const
{
    return m_height;
}