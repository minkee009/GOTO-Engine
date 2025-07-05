#include "WStringHelper.h"
#include <sstream>

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