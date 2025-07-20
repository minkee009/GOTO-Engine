#pragma once
#include <string>
#include <vector>

namespace GOTOEngine
{
	class WStringHelper
	{
	public:
		static std::vector<std::wstring> Split(const std::wstring& str, wchar_t delim);
		static std::wstring string_to_wstring(const std::string& str);
		static std::string wstring_to_string(const std::wstring& wstr);
	};
}

#define STR_TO_WSTR GOTOEngine::WStringHelper::string_to_wstring
#define WSTR_TO_STR GOTOEngine::WStringHelper::wstring_to_string