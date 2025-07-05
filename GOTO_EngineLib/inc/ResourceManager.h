#pragma once
#include <Singleton.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <Color.h>
#include <wincodec.h>



namespace GOTOEngine
{
	class IRenderImage;
	class IRenderFont;
	enum IRenderFontStyle;
	struct ResourceEntry;
	class Engine;
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		void StartUp();
		void ShutDown();
		IRenderImage* LoadStaticImage(const std::wstring& filePath);
		IRenderFont* LoadStaticFont(const std::wstring& fontFamily,IRenderFontStyle fontStyle,int size);
	private:
		friend class Engine;
		static std::unordered_map<std::wstring, ResourceEntry> m_resources;
	};
}
