#pragma once
#include <wincodec.h>
#include <wrl/client.h>

namespace GOTOEngine
{
    class WICHelper
    {
    public:
        static void StartUp();
        static IWICImagingFactory* GetFactory();
        static void ShutDown() { s_factory = nullptr; }

    private:
        static Microsoft::WRL::ComPtr<IWICImagingFactory> s_factory;
    };
}