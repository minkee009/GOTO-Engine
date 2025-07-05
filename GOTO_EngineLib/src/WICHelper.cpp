#include "WICHelper.h"
#include <stdexcept>

using Microsoft::WRL::ComPtr;

using namespace GOTOEngine;

ComPtr<IWICImagingFactory> WICHelper::s_factory;

void GOTOEngine::WICHelper::StartUp()
{
    if (!s_factory)
    {
        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&s_factory));
        if (FAILED(hr))
            throw std::runtime_error("WICImagingFactory 생성 실패");
    }
}

IWICImagingFactory* WICHelper::GetFactory()
{
    return s_factory.Get();
}