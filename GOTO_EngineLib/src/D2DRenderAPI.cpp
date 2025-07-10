#include "D2DRenderAPI.h"
#include "D2DImage.h"
#include "D2DFont.h"
#include <d2d1helper.h>
#ifdef _DEBUG
#include <iostream>
#include "InputManager.h"
#include "TimeManager.h"
#include "Mathf.h"
#endif

using namespace GOTOEngine;

bool D2DRenderAPI::Initialize(IWindow* window)
{
	if (!window)
		return false;

	m_window = window;

	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, m_d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;
	m_d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));


	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = m_window->GetWidth();
	scDesc.Height = m_window->GetHeight();
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), static_cast<HWND>(m_window->GetNativeHandle()), &scDesc, nullptr, nullptr, m_swapChain.GetAddressOf());

	// 백버퍼를 타겟으로 설정
	ComPtr<IDXGISurface> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_renderTarget.GetAddressOf());
	m_d2dContext->SetTarget(m_renderTarget.Get());

	m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_solidColorBrush);

	m_pGpuResourcesMap = new std::unordered_map<std::wstring, ComPtr<ID2D1Bitmap1>>();
	return true;
}

void D2DRenderAPI::Release()
{
	delete m_pGpuResourcesMap;

	m_d3dDevice = nullptr;
	m_swapChain = nullptr;
	m_d2dContext = nullptr;
	m_renderTarget = nullptr;
	m_d2dFactory = nullptr;
	m_solidColorBrush = nullptr;
	m_pGpuResourcesMap = nullptr;
}

void D2DRenderAPI::ChangeBufferSize(int newWidth, int newHeight)
{
	if (!m_swapChain || !m_d2dContext)
		return;

	// 현재 렌더 타겟 연결 해제
	m_d2dContext->SetTarget(nullptr);
	m_renderTarget = nullptr;

	// SwapChain 버퍼 크기 변경
	HRESULT hr = m_swapChain->ResizeBuffers(
		2, // 버퍼 개수 (CreateSwapChainForHwnd에서 설정한 값과 같아야 함)
		newWidth,
		newHeight,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		0
	);

	if (FAILED(hr)) {
		OutputDebugStringW(L"ResizeBuffers 실패\n");
		return;
	}

	// 백버퍼 가져오기
	ComPtr<IDXGISurface> backBuffer;
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputDebugStringW(L"GetBuffer 실패\n");
		return;
	}

	// 렌더 타겟 다시 생성
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	hr = m_d2dContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_renderTarget.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugStringW(L"렌더 타겟 생성 실패\n");
		return;
	}

	m_d2dContext->SetTarget(m_renderTarget.Get());
}

void D2DRenderAPI::Clear()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void D2DRenderAPI::DrawImage(int x, int y, float scale, bool flipX, const IRenderImage* image)
{
	if (!image || !m_d2dContext)
		return;

	auto d2dImage = dynamic_cast<const D2DImage*>(image);
	if (!d2dImage)
		return;

	ComPtr<ID2D1Bitmap1> d2dBitmap;
	const std::wstring& filePath = d2dImage->GetFilePath();

	//이미지 가져오기
	auto it = m_pGpuResourcesMap->find(filePath);
	if (it == m_pGpuResourcesMap->end())
	{
		const std::vector<BYTE>* imageData = static_cast<const std::vector<BYTE>*>(d2dImage->GetNativeHandle());
		if (!imageData) return;

		D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_NONE,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		HRESULT hr = m_d2dContext->CreateBitmap(
			D2D1::SizeU(d2dImage->GetSrcWidth(), d2dImage->GetSrcHeight()),
			imageData->data(),
			d2dImage->GetSrcWidth() * 4,
			&props,
			d2dBitmap.GetAddressOf()
		);

		if (FAILED(hr))
		{
			OutputDebugStringW(L"이미지 GPU로드 실패\n");
			return;
		}
		(*m_pGpuResourcesMap)[filePath] = d2dBitmap;
	}
	else
	{
		d2dBitmap = it->second;
	}

	auto size = d2dBitmap->GetSize();

	//이미지 그리기
	D2D1_RECT_F destinationRect = D2D1::RectF(
		static_cast<FLOAT>(x),
		static_cast<FLOAT>(y),
		static_cast<FLOAT>(x + d2dImage->GetWidth()),// * scale,
		static_cast<FLOAT>(y + d2dImage->GetHeight())// * scale
	);

	D2D1_RECT_F sourceRect = D2D1::RectF(
		static_cast<FLOAT>(d2dImage->GetPartialX()),
		static_cast<FLOAT>(d2dImage->GetPartialY()),
		static_cast<FLOAT>(d2dImage->GetPartialX() + d2dImage->GetWidth()),
		static_cast<FLOAT>(d2dImage->GetPartialY() + d2dImage->GetHeight())
	);


#ifdef _DEBUG
	D2D_MATRIX_3X2_F Mat = D2D1::IdentityMatrix(); //D2D1::Matrix3x2F::Translate(-d2dImage->GetPivotX(), -d2dImage->GetPivotY());


	//이미지 피벗 적용
	//Mat = Mat * D2D1::Matrix3x2F::Translation(-d2dImage->GetPivotX(), -d2dImage->GetPivotY());
	if (INPUT_GET_KEY(KeyCode::F))
	{
		//디버그용 코드
	}

	static float sinT = 0.0f;
	sinT = (sinf(2.0f * Mathf::PI * TIME_GET_TOTALTIME()) + 1.0f) * 0.5f;

	static auto trans = 0.0f;
	static auto rot = FLOAT{ 0.0f };
	
	trans = sinT * 25.0f;
	rot += 12.5f * TIME_GET_DELTATIME();

	//유니티 좌표계 이미지 플립
	Mat = Mat * D2D1::Matrix3x2F::Scale(1.0f, -1.0f);

	//TRS 세팅
	Mat = Mat * D2D1::Matrix3x2F::Scale(scale ,scale) * D2D1::Matrix3x2F::Rotation({ rot }) * D2D1::Matrix3x2F::Translation(0.0f, trans * scale);

	//유니티 좌표계 매트릭스 적용
	Mat = Mat * D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation(m_window->GetWidth() * 0.5f, m_window->GetHeight() * 0.5f);

	if (INPUT_GET_KEY(KeyCode::Space))
	{
		//디버그용 코드
	}

	m_d2dContext->SetTransform(Mat);
#endif // DEBUG

	m_d2dContext->DrawBitmap(
		d2dBitmap.Get(),
		&destinationRect, 
		1.0f, // 불투명도
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		&sourceRect
	);
}

void D2DRenderAPI::DrawString(int x, int y, int width, int height, const wchar_t* string, const GOTOEngine::IRenderFont* font, bool rightAlign, Color color)
{
	if (!font || !string || !m_d2dContext)
		return;

	IDWriteTextFormat* textFormat = static_cast<IDWriteTextFormat*>(font->GetNativeHandle());
	if (!textFormat)
		return;

	m_solidColorBrush->SetColor(D2D1::ColorF(color.R, color.G, color.B, color.A));

	// 정렬 설정
	textFormat->SetTextAlignment(rightAlign ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	D2D1_RECT_F layoutRect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(width), static_cast<FLOAT>(height));

	m_d2dContext->SetTransform(D2D1::IdentityMatrix());
	m_d2dContext->DrawText(string, static_cast<UINT32>(wcslen(string)), textFormat, &layoutRect, m_solidColorBrush.Get());
}

void D2DRenderAPI::DrawRect(int x, int y, int width, int height, bool fill, Color color)
{
	if (!m_solidColorBrush) {
		OutputDebugStringA("SolidColorBrush가 초기화되지 않았습니다.\n");
		return;
	}
	m_d2dContext->SetTransform(D2D1::IdentityMatrix());
	m_solidColorBrush->SetColor(D2D1::ColorF(color.R, color.G, color.B, color.A));
	if (fill) {
		m_d2dContext->FillRectangle(D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), 
			static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height)), m_solidColorBrush.Get());
	}
	else {
		m_d2dContext->DrawRectangle(D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), 
			static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height)), m_solidColorBrush.Get());
	}
}

void D2DRenderAPI::SwapBuffer()
{
	m_d2dContext->EndDraw();
    m_swapChain->Present(m_vSyncInterval, 0); // vsync 켜고 1 프레임 기다림
}

D2DRenderAPI::~D2DRenderAPI()
{
	Release();
}
