#pragma once
#include <dwrite.h>
#include <wrl/client.h>

namespace GOTOEngine
{
    class DWriteHelper
    {
    public:
        static void StartUp();
        static IDWriteFactory* GetFactory();
        static void ShutDown() { s_factory = nullptr; }

    private:
        static Microsoft::WRL::ComPtr<IDWriteFactory> s_factory;
    };
}