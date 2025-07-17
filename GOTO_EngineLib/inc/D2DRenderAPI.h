#pragma once
#include <unordered_map>
#include <string>
#include <windows.h>
#include <assert.h>
#include <wrl.h>  // ComPtr ����� ���� ���

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#include "IRenderAPI.h"
#include "IWindow.h"
#include "Matrix3x3.h"

using namespace Microsoft::WRL;

namespace GOTOEngine
{
    class D2DRenderAPI : public IRenderAPI
    {
    public:
        bool Initialize(IWindow* window) override;
        void Release() override;
        void ChangeBufferSize(int newWidth, int newHeight) override;
        void ClearGPUResourcesMap() { m_pGpuResourcesMap->clear(); }
        void Clear() override;
        //void DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image) override;
        void DrawBitmap(const Matrix3x3& mat, const IRenderBitmap* bitmap, const Rect& sourceRect) override;
        void DrawString(float x, float y, float width, float height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color) override;
        void DrawRect(float x, float y, float width, float height, bool fill, Color color) override;

        void SetViewport(Rect rect) override;
        void ResetViewport() override;
        IRenderBitmap* CreateRenderBitmap(std::wstring filePath);
        IRenderFont* CreateRenderFontFromFilePath(std::wstring filePath);
        IRenderFont* CreateRenderFontFromOS(std::wstring fontName);

        void SwapBuffer() override;
        IWindow& GetWindow() const override { return *m_window; }
        void SetVSyncInterval(int interval) override { m_vSyncInterval = interval; }
        ~D2DRenderAPI() override;

        ID2D1DeviceContext7* GetContext() { return m_d2dContext.Get(); }
        ID2D1Bitmap1* GetRenderTarget() { return m_renderTarget.Get(); }

        const D2D1_RECT_F& GetClipRect() const { return m_clipRect; }

        D2D1::Matrix3x2F ConvertToD2DMatrix(const Matrix3x3& mat)
        {
            return D2D1::Matrix3x2F(
                mat.m[0][0], mat.m[0][1], // m11, m12 (ù ��° ��)
                mat.m[1][0], mat.m[1][1], // m21, m22 (�� ��° ��)  
                mat.m[2][0], mat.m[2][1]  // dx, dy (��ȯ ����)
            );
        }

        std::string FormatBytes(UINT64 bytes);
        RenderAPIMemoryStatus CollectMemoryUsage() override;
    private:

        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<IDXGISwapChain1> m_swapChain;
        ComPtr<ID2D1DeviceContext7> m_d2dContext;
        ComPtr<ID2D1Bitmap1> m_renderTarget;
        ComPtr<ID2D1Factory8> m_d2dFactory;

        ComPtr<IDXGIAdapter3> m_dxgiAdapter;
        ComPtr<IDXGIDevice3> m_dxgiDevice;

        ComPtr<ID2D1SolidColorBrush> m_solidColorBrush;

        IWindow* m_window;
        D2D1_RECT_F m_clipRect;

        IRenderFont* m_defaultFont;

		int m_vSyncInterval = 1; // �⺻ VSync ����

        std::unordered_map<std::wstring, ComPtr<ID2D1Bitmap1>>* m_pGpuResourcesMap; // <filepath, converted_image>
    };
}

