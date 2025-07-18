#pragma once
#include <dwrite.h>
#include <dwrite_3.h>
#include <wrl/client.h>
#pragma comment(lib, "dwrite.lib")

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