#include "DWriteHelper.h"
#include <stdexcept>

using namespace GOTOEngine;
using Microsoft::WRL::ComPtr;

ComPtr<IDWriteFactory3> DWriteHelper::s_factory;

void GOTOEngine::DWriteHelper::StartUp()
{
    if (!s_factory)
    {
        ComPtr<IDWriteFactory> baseFactory;
        HRESULT hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(baseFactory.GetAddressOf())
        );

        if (FAILED(hr))
            throw std::runtime_error("DWriteFactory 생성 실패");

        // IDWriteFactory3 쿼리
        hr = baseFactory.As(&s_factory);
        if (FAILED(hr))
            throw std::runtime_error("IDWriteFactory3로 변환 실패");
    }
}

IDWriteFactory3* DWriteHelper::GetFactory()
{
    return s_factory.Get();
}
