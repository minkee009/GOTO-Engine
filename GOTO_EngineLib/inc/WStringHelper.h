#pragma once
#include <string>
#include <vector>

namespace GOTOEngine
{
	class WStringHelper
	{
	public:
		static std::vector<std::wstring> Split(const std::wstring& str, wchar_t delim);
	};
}