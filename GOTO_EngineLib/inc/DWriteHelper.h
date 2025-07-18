#pragma once
#include <dwrite.h>
#include <dwrite_3.h>
#include <wrl/client.h>

namespace GOTOEngine
{
    class DWriteHelper
    {
    public:
        static void StartUp();
        static IDWriteFactory3* GetFactory();
        static void ShutDown() { s_factory = nullptr; }

    private:
        static Microsoft::WRL::ComPtr<IDWriteFactory3> s_factory;
    };
}