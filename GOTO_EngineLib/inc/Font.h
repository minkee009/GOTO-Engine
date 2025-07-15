#pragma once
#include <Resource.h>

namespace GOTOEngine
{
	class Font : public Resource
	{
	private:
		friend class ResourceManager;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidRawData() override;
	public:

	};
}