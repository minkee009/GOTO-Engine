#include "WStringHelper.h"
#include <sstream>
#include <locale>
#include <codecvt>

std::vector<std::wstring> GOTOEngine::WStringHelper::Split(const std::wstring& str, wchar_t delim)
{
    std::vector<std::wstring> tokens;
    std::wistringstream stream(str);
    std::wstring token;
    while (std::getline(stream, token, delim))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

std::wstring GOTOEngine::WStringHelper::string_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string GOTOEngine::WStringHelper::wstring_to_string(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}