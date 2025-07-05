#include "DWriteHelper.h"
#include <stdexcept>

using namespace GOTOEngine;
using Microsoft::WRL::ComPtr;

ComPtr<IDWriteFactory> DWriteHelper::s_factory;

void GOTOEngine::DWriteHelper::StartUp()
{
    if (!s_factory)
    {
        HRESULT hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(s_factory.GetAddressOf())
        );

        if (FAILED(hr))
            throw std::runtime_error("DWriteFactory 생성 실패");
    }
}

IDWriteFactory* DWriteHelper::GetFactory()
{
    return s_factory.Get();
}
