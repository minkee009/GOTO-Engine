#pragma once
#include <unordered_map>
#include <string>
#include <windows.h>
#include <assert.h>
#include <wrl.h>  // ComPtr 사용을 위한 헤더

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
        void DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image) override;
        void DrawString(int x, int y, int width, int height, const wchar_t* string, const IRenderFont* font, bool rightAlign, Color color) override;
        void DrawRect(int x, int y, int width, int height, bool fill, Color color) override;

        void SetViewport(Rect rect) override;
        void ResetViewport() override;

        void SwapBuffer() override;
		IWindow& GetWindow() const override { return *m_window; }
        void SetVSyncInterval(int interval) override { m_vSyncInterval = interval; }
        ~D2DRenderAPI() override;

        D2D1::Matrix3x2F ConvertToD2DMatrix(const Matrix3x3& mat)
        {
            return D2D1::Matrix3x2F(
                mat.m[0][0], mat.m[0][1], // _11, _12
                mat.m[1][0], mat.m[1][1], // _21, _22
                mat.m[2][0], mat.m[2][1]  // _31, _32 (Translation)
            );
        }
    private:

        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<IDXGISwapChain1> m_swapChain;
        ComPtr<ID2D1DeviceContext7> m_d2dContext;
        ComPtr<ID2D1Bitmap1> m_renderTarget;
        ComPtr<ID2D1Factory8> m_d2dFactory;

        ComPtr<ID2D1SolidColorBrush> m_solidColorBrush;

        IWindow* m_window;
        D2D1_RECT_F m_clipRect;

		int m_vSyncInterval = 1; // 기본 VSync 간격

        std::unordered_map<std::wstring, ComPtr<ID2D1Bitmap1>>* m_pGpuResourcesMap; // <filepath, converted_image>
    };
}

