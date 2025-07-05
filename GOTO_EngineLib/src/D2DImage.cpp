#include "D2DImage.h"
#include "WICHelper.h"
#include <wincodec.h>
#include <stdexcept>
#include <wrl/client.h> // for Microsoft::WRL::ComPtr

using Microsoft::WRL::ComPtr;
using namespace GOTOEngine;

D2DImage::D2DImage(const std::wstring& filePath, int pivotX, int pivotY) 
    : m_filePath(filePath)
{
    // 1. Factory 재사용
    IWICImagingFactory* factory = WICHelper::GetFactory();

    // 2. Decoder 생성
    ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(factory->CreateDecoderFromFilename(
        m_filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
        throw std::runtime_error("Decoder 로드 실패");

    // 3. 첫 번째 프레임 가져오기
    ComPtr<IWICBitmapFrameDecode> frame;
    if (FAILED(decoder->GetFrame(0, &frame)))
        throw std::runtime_error("first frame 로드 실패");

    // 4. 원하는 픽셀 포맷으로 변환 (32bpp BGRA)
    ComPtr<IWICFormatConverter> converter;
    if (FAILED(factory->CreateFormatConverter(&converter)))
        throw std::runtime_error("fomatconverter 로드 실패");

    if (FAILED(converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppBGRA,
        WICBitmapDitherTypeNone,
        nullptr, 0.0, WICBitmapPaletteTypeCustom)))
        throw std::runtime_error("fomatting 실패");

    // 5. 이미지 크기 얻기
    UINT width, height;
    if (FAILED(converter->GetSize(&width, &height)))
        throw std::runtime_error("이미지 크기 얻기 실패");

    m_image = new std::vector<BYTE>();

    m_width = width;
    m_height = height;
    m_srcWidth = width;
    m_srcHeight = height;
    m_image->resize(width * height * 4); // 4바이트 per 픽셀 (RGBA)

    // 6. 픽셀 복사
    if (FAILED(converter->CopyPixels(
        nullptr,
        width * 4, // stride
        static_cast<UINT>(m_image->size()),
        m_image->data())))
        throw std::runtime_error("픽셀 복사 실패");
}

D2DImage::D2DImage(IRenderImage* imageAtlas, int partialX, int partialY, int width, int height, int pivotX, int pivotY)
    : m_filePath(imageAtlas->GetFilePath())
    , m_partialX(partialX)
    , m_partialY(partialY)
    , m_width(width)
    , m_height(height)
    , m_srcWidth(imageAtlas->GetWidth())
    , m_srcHeight(imageAtlas->GetHeight())
{
    m_image = static_cast<std::vector<BYTE>*>(imageAtlas->GetNativeHandle());
    m_isPartial = true;
}

D2DImage::~D2DImage()
{
	if (!m_isPartial)
	{
		delete m_image;
		m_image = nullptr;
	}
}