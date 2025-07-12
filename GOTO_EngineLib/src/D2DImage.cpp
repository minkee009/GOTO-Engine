#include "D2DImage.h"
#include "WICHelper.h"
#include <wincodec.h>
#include <stdexcept>
#include <wrl/client.h> // for Microsoft::WRL::ComPtr

using Microsoft::WRL::ComPtr;
using namespace GOTOEngine;

D2DImage::D2DImage(const std::wstring& filePath) 
    : m_filePath(filePath)
{
    // 1. Factory ����
    IWICImagingFactory* factory = WICHelper::GetFactory();

    // 2. Decoder ����
    ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(factory->CreateDecoderFromFilename(
        m_filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
        throw std::runtime_error("Decoder �ε� ����");

    // 3. ù ��° ������ ��������
    ComPtr<IWICBitmapFrameDecode> frame;
    if (FAILED(decoder->GetFrame(0, &frame)))
        throw std::runtime_error("first frame �ε� ����");

    // 4. ���ϴ� �ȼ� �������� ��ȯ (32bpp BGRA)
    ComPtr<IWICFormatConverter> converter;
    if (FAILED(factory->CreateFormatConverter(&converter)))
        throw std::runtime_error("fomatconverter �ε� ����");

    if (FAILED(converter->Initialize(
        frame.Get(),
        GUID_WICPixelFormat32bppBGRA,
        WICBitmapDitherTypeNone,
        nullptr, 0.0, WICBitmapPaletteTypeCustom)))
        throw std::runtime_error("fomatting ����");

    // 5. �̹��� ũ�� ���
    UINT width, height;
    if (FAILED(converter->GetSize(&width, &height)))
        throw std::runtime_error("�̹��� ũ�� ��� ����");

    m_image = new std::vector<BYTE>();

    m_width = width;
    m_height = height;
    m_image->resize(width * height * 4); // 4����Ʈ per �ȼ� (RGBA)

    // 6. �ȼ� ����
    if (FAILED(converter->CopyPixels(
        nullptr,
        width * 4, // stride
        static_cast<UINT>(m_image->size()),
        m_image->data())))
        throw std::runtime_error("�ȼ� ���� ����");
}

D2DImage::~D2DImage()
{
    delete m_image;
    m_image = nullptr;
}