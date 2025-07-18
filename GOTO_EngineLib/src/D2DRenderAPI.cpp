#include <d2d1helper.h>
#include <WICHelper.h>
#include "DWriteHelper.h"
#include "D2DRenderAPI.h"
#include "D2DFont.h"
#include "D2DBitmap.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <psapi.h>                // GetProcessMemoryInfo, PROCESS_MEMORY_COUNTERS_EX
#pragma comment(lib, "psapi.lib")
//#ifdef _DEBUG
//#include <iostream>
//#endif

using namespace GOTOEngine;

bool D2DRenderAPI::Initialize(IWindow* window)
{
	if (!window)
		return false;

	m_window = window;

	// D3D11 ����̽� ����
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D ���丮 �� ����̽�
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, m_d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);
	ComPtr<IDXGIAdapter> dxgiAdapter;
	m_d3dDevice.As(&m_dxgiDevice);
	m_dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
	dxgiAdapter.As(&m_dxgiAdapter);

	ComPtr<ID2D1Device7> d2dDevice;
	m_d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));


	// SwapChain ����
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = m_window->GetWidth();
	scDesc.Height = m_window->GetHeight();
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), static_cast<HWND>(m_window->GetNativeHandle()), &scDesc, nullptr, nullptr, m_swapChain.GetAddressOf());

	// ����۸� Ÿ������ ����
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
	if (m_defaultFont)
		delete m_defaultFont;

	m_d3dDevice = nullptr;
	m_dxgiAdapter = nullptr;
	m_dxgiDevice = nullptr;
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

	// ���� ���� Ÿ�� ���� ����
	m_d2dContext->SetTarget(nullptr);
	m_renderTarget = nullptr;

	// SwapChain ���� ũ�� ����
	HRESULT hr = m_swapChain->ResizeBuffers(
		2, // ���� ���� (CreateSwapChainForHwnd���� ������ ���� ���ƾ� ��)
		newWidth,
		newHeight,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		0
	);

	if (FAILED(hr)) {
		OutputDebugStringW(L"ResizeBuffers ����\n");
		return;
	}

	// ����� ��������
	ComPtr<IDXGISurface> backBuffer;
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputDebugStringW(L"GetBuffer ����\n");
		return;
	}

	// ���� Ÿ�� �ٽ� ����
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	hr = m_d2dContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_renderTarget.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugStringW(L"���� Ÿ�� ���� ����\n");
		return;
	}

	m_d2dContext->SetTarget(m_renderTarget.Get());
}

void D2DRenderAPI::Clear()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void GOTOEngine::D2DRenderAPI::DrawBitmap(const IRenderBitmap* bitmap, const Matrix3x3& mat, const Rect& sourceRect)
{
	auto d2dTransform = ConvertToD2DMatrix(mat);
	auto d2dBitmap = static_cast<D2DBitmap*>(const_cast<IRenderBitmap*>(bitmap))->GetRaw();

	D2D1_RECT_F destRect = D2D1::RectF(
		0.0f,
		0.0f,
		sourceRect.width,
		sourceRect.height
	);

	auto d2dDestY = bitmap->GetHeight() - sourceRect.y - sourceRect.height;

	D2D1_RECT_F srcRect = D2D1::RectF(
		sourceRect.x,
		d2dDestY,
		sourceRect.x + sourceRect.width,
		d2dDestY + sourceRect.height
	);

	m_d2dContext->SetTransform(d2dTransform);
	m_d2dContext->DrawBitmap(
		d2dBitmap,
		&destRect,
		1.0f, // ������
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, 
		// D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
		&srcRect
	);
}

void D2DRenderAPI::DrawString(const wchar_t* string, const Rect& rect, const IRenderFont* font, size_t size, const IRenderFontStyle& fontStyle, Color color, const Matrix3x3& mat, int hAlignment, int vAlignment, bool useScreenPos)
{
	if (!string || !m_d2dContext)
		return;

	if (!font)
	{
		if (!m_defaultFont)
			m_defaultFont = new D2DFont(L"Segoe UI");

		font = m_defaultFont;
	}

	IDWriteTextFormat* textFormat = static_cast<D2DFont*>(const_cast<IRenderFont*>(font))->GetRaw(size, fontStyle);
	if (!textFormat)
		return;

	m_solidColorBrush->SetColor(D2D1::ColorF(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f));

	// ���� ����
	switch (hAlignment)
	{
	case -1:
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case 0:
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); 
		break;
	case 1:
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	}

	switch (vAlignment)
	{
	case -1:
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case 0:
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	case 1:
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	}

	float screenHeight = static_cast<float>(m_window->GetHeight());

	D2D1_RECT_F layoutRect; 
	
	if (useScreenPos)
	{
		layoutRect = D2D1::RectF(rect.x, (screenHeight - rect.y - rect.height), (rect.x + rect.width), (screenHeight - rect.y));
	}
	else
	{
		layoutRect = D2D1::RectF(0.0f, 0.0f, rect.width, rect.height);
	}

	auto d2dTransform = ConvertToD2DMatrix(mat);

	m_d2dContext->SetTransform(d2dTransform);
	m_d2dContext->DrawText(string, static_cast<UINT32>(wcslen(string)), textFormat, &layoutRect, m_solidColorBrush.Get());
}

void GOTOEngine::D2DRenderAPI::DrawRect(const Rect& rect, bool fill, Color color)
{
	if (!m_solidColorBrush) {
		OutputDebugStringA("SolidColorBrush�� �ʱ�ȭ���� �ʾҽ��ϴ�.\n");
		return;
	}
	float screenHeight = static_cast<float>(m_window->GetHeight());
	m_d2dContext->SetTransform(D2D1::IdentityMatrix());
	m_solidColorBrush->SetColor(D2D1::ColorF(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f));
	if (fill) {
		m_d2dContext->FillRectangle(D2D1::RectF(rect.x, (screenHeight - rect.y - rect.height),
			(rect.x + rect.width), (screenHeight - rect.y)), m_solidColorBrush.Get());
	}
	else {
		m_d2dContext->DrawRectangle(D2D1::RectF(rect.x, (screenHeight - rect.y - rect.height),
			(rect.x + rect.width), (screenHeight - rect.y)), m_solidColorBrush.Get());
	}
}

void D2DRenderAPI::SetViewport(Rect rect)
{
	m_d2dContext->SetTransform(D2D1::IdentityMatrix());
	float screenWidth = static_cast<float>(m_window->GetWidth());
	float screenHeight = static_cast<float>(m_window->GetHeight());

	m_clipRect = D2D1::RectF(
		rect.x * screenWidth,                           // ���� x ��ǥ (�ȼ�)
		(1.0f - (rect.y + rect.height)) * screenHeight, // ��� y ��ǥ (�ȼ�)
		(rect.x + rect.width) * screenWidth,            // ���� x ��ǥ (�ȼ�)
		(1.0f - rect.y) * screenHeight                  // �ϴ� y ��ǥ (�ȼ�)
	);

	m_d2dContext->PushAxisAlignedClip(
		m_clipRect,
		D2D1_ANTIALIAS_MODE_ALIASED
		// D2D1_ANTIALIAS_MODE_PER_PRIMITIVE // Ŭ�� ��迡 ��Ƽ�ٸ���� ����
		// D2D1_ANTIALIAS_MODE_ALIASED // �ȼ� �Ϻ��� Ŭ������ �ʿ��� ��� (���ɻ� ���� ���� �� ����)
	);
}

void GOTOEngine::D2DRenderAPI::ResetViewport()
{
	m_d2dContext->PopAxisAlignedClip();
}

IRenderBitmap* GOTOEngine::D2DRenderAPI::CreateRenderBitmap(std::wstring filePath)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;

	HRESULT hr = WICHelper::GetFactory()->CreateDecoderFromFilename(
		filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return nullptr;

	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return nullptr;

	hr = WICHelper::GetFactory()->CreateFormatConverter(&converter);
	if (FAILED(hr)) return nullptr;

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return nullptr;

	// �� Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	ComPtr<ID2D1Bitmap1> bitmap;

	hr = m_d2dContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, bitmap.GetAddressOf());

	if (FAILED(hr)) return nullptr;

	return new D2DBitmap(bitmap);
}

IRenderFont* GOTOEngine::D2DRenderAPI::CreateRenderFontFromFilePath(std::wstring filePath)
{
	IDWriteFactory3* dwriteFactory = DWriteHelper::GetFactory();
	if (!dwriteFactory)
		return nullptr;

	// 1. FontSetBuilder (IDWriteFontSetBuilder1) ����
	ComPtr<IDWriteFontSetBuilder> baseBuilder;
	HRESULT hr = dwriteFactory->CreateFontSetBuilder(&baseBuilder);
	if (FAILED(hr)) return nullptr;

	ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
	hr = baseBuilder.As(&fontSetBuilder);
	if (FAILED(hr)) return nullptr;

	// 2. FontFile ����
	ComPtr<IDWriteFontFile> fontFile;
	hr = dwriteFactory->CreateFontFileReference(filePath.c_str(), nullptr, &fontFile);
	if (FAILED(hr)) return nullptr;

	// 3. FontFile�� FontSetBuilder�� �߰�
	hr = fontSetBuilder->AddFontFile(fontFile.Get());
	if (FAILED(hr)) return nullptr;

	// 4. FontSet ����
	ComPtr<IDWriteFontSet> fontSet;
	hr = fontSetBuilder->CreateFontSet(&fontSet);
	if (FAILED(hr)) return nullptr;

	// 5. FontCollection ����
	ComPtr<IDWriteFontCollection1> fontCollection;
	hr = dwriteFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);
	if (FAILED(hr)) return nullptr;

	// 6. FontFamily �̸� ����
	ComPtr<IDWriteFontFamily1> fontFamily;
	hr = fontCollection->GetFontFamily(0, &fontFamily);
	if (FAILED(hr)) return nullptr;

	ComPtr<IDWriteLocalizedStrings> familyNames;
	hr = fontFamily->GetFamilyNames(&familyNames);
	if (FAILED(hr)) return nullptr;

	UINT32 index = 0;
	BOOL exists = FALSE;
	hr = familyNames->FindLocaleName(L"en-us", &index, &exists);
	if (!exists) index = 0;

	UINT32 length = 0;
	hr = familyNames->GetStringLength(index, &length);
	if (FAILED(hr)) return nullptr;

	std::wstring fontFamilyName(length + 1, L'\0');
	hr = familyNames->GetString(index, &fontFamilyName[0], length + 1);
	if (FAILED(hr)) return nullptr;

	fontFamilyName.resize(length);

	// 7. D2DFont ���� �� ��ȯ
	D2DFont* d2dFont = new D2DFont(fontFamilyName);
	d2dFont->m_fontCollection = fontCollection;
	d2dFont->m_fontFile = fontFile; // IDWriteFontFile���� ������

	return d2dFont;
}

IRenderFont* GOTOEngine::D2DRenderAPI::CreateRenderFontFromOS(std::wstring fontName)
{
	IDWriteFactory* dwriteFactory = DWriteHelper::GetFactory();
	// DWriteHelper�� �ʱ�ȭ���� ���� ��� ����
	if (!dwriteFactory)
	{
		return nullptr;
	}

	// �ý��� ��Ʈ �÷��� ��������
	ComPtr<IDWriteFontCollection> systemFontCollection;
	HRESULT hr = dwriteFactory->GetSystemFontCollection(
		systemFontCollection.GetAddressOf()
	);

	if (FAILED(hr))
		return nullptr;

	// ��Ʈ �йи� ã��
	UINT32 index = 0;
	BOOL exists = FALSE;
	hr = systemFontCollection->FindFamilyName(
		fontName.c_str(),
		&index,
		&exists
	);

	if (FAILED(hr) || !exists)
	{
		// ��Ʈ�� ã�� ������ ��� �⺻ ��Ʈ ���
		fontName = L"Segoe UI";
		hr = systemFontCollection->FindFamilyName(
			fontName.c_str(),
			&index,
			&exists
		);

		if (FAILED(hr) || !exists)
			return nullptr;
	}

	// D2DFont ��ü ���� (�ý��� ��Ʈ�� ��� fontFile�� fontCollection�� nullptr)
	D2DFont* d2dFont = new D2DFont(fontName);

	return d2dFont;
}

void D2DRenderAPI::SwapBuffer()
{
	m_d2dContext->EndDraw();
    m_swapChain->Present(m_vSyncInterval, 0); // vsync �Ѱ� 1 ������ ��ٸ�
}

D2DRenderAPI::~D2DRenderAPI()
{
	Release();
}

std::string GOTOEngine::D2DRenderAPI::FormatBytes(UINT64 bytes)
{
	constexpr double KB = 1024.0;
	constexpr double MB = KB * 1024.0;
	constexpr double GB = MB * 1024.0;

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2);

	if (bytes >= static_cast<UINT64>(GB))
		oss << (bytes / GB) << " GB";
	else if (bytes >= static_cast<UINT64>(MB))
		oss << (bytes / MB) << " MB";
	else if (bytes >= static_cast<UINT64>(KB))
		oss << (bytes / KB) << " KB";
	else
		oss << bytes << " B";

	return oss.str();
}

RenderAPIMemoryStatus GOTOEngine::D2DRenderAPI::CollectMemoryUsage()
{
	RenderAPIMemoryStatus status;

	DXGI_QUERY_VIDEO_MEMORY_INFO memInfo = {};
	m_dxgiAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
	status.vramUsage = FormatBytes(memInfo.CurrentUsage);

	HANDLE hProcess = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX pmc;
	pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);

	// ���� ���μ����� �޸� ��� ���� ��ȸ
	GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	status.dramUsage = FormatBytes(pmc.WorkingSetSize);
	status.pageFileUsage = FormatBytes(pmc.PagefileUsage - pmc.WorkingSetSize);

	return status;
}
