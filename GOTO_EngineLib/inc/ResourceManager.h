#pragma once
#include <Singleton.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <Color.h>
#include <wincodec.h>
#include <typeindex>


namespace GOTOEngine
{
	class IRenderImage;
	class IRenderFont;
	enum IRenderFontStyle;
	class Engine;

	struct ResourceEntry {
		std::shared_ptr<void> data;
		std::type_index type;

		ResourceEntry() : data(nullptr), type(typeid(void)) {}
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		IRenderImage* LoadStaticImage(const std::wstring& filePath);
		IRenderFont* LoadStaticFont(const std::wstring& fontFamily,IRenderFontStyle fontStyle,int size);
	private:
		friend class Engine;
		std::unordered_map<std::wstring, ResourceEntry> m_resources;

		void StartUp();
		void ShutDown();
	};
}
