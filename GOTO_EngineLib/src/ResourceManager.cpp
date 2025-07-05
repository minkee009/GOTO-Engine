#include "ResourceManager.h"
#include "IRenderImage.h"
#include "D2DImage.h"
#include "D2DFont.h"
#include <typeindex>

using namespace GOTOEngine;

struct GOTOEngine::ResourceEntry {
	std::shared_ptr<void> data;
	std::type_index type;

	ResourceEntry() : data(nullptr), type(typeid(void)) {}
};

std::unordered_map<std::wstring, ResourceEntry> ResourceManager::m_resources;

void ResourceManager::StartUp()
{
	m_resources.clear();
}

void ResourceManager::ShutDown()
{
	m_resources.clear();
}

IRenderImage* ResourceManager::LoadStaticImage(const std::wstring& filePath)
{
	auto it = m_resources.find(filePath);
	if (it != m_resources.end())
	{
		if (it->second.type == typeid(IRenderImage)) {
			return static_cast<IRenderImage*>(it->second.data.get());
		}
		return nullptr; // Ÿ�� ����ġ - �ٸ� Ÿ������ �̹� �ε�� ���
	}

	std::shared_ptr<IRenderImage> image = std::make_shared<D2DImage>(filePath);
	if (!image) // ���� ���� ��
		return nullptr;

	// ĳ�ÿ� ����
	ResourceEntry entry;
	entry.data = image;
	entry.type = typeid(IRenderImage);
	m_resources[filePath] = entry;

	return image.get();
}

IRenderFont* GOTOEngine::ResourceManager::LoadStaticFont(const std::wstring& fontFamily,IRenderFontStyle fontStyle, int size)
{
	int styleValue = static_cast<int>(fontStyle);

	std::wstring hashString = fontFamily + L"_" + std::to_wstring(styleValue) +
		L"_" + std::to_wstring(size);

	auto it = m_resources.find(hashString);
	if (it != m_resources.end())
	{
		if (it->second.type == typeid(IRenderFont)) {
			return static_cast<IRenderFont*>(it->second.data.get());
		}
		return nullptr; // Ÿ�� ����ġ - �ٸ� Ÿ������ �̹� �ε�� ���
	}

	std::shared_ptr<IRenderFont> font = std::make_shared<D2DFont>(fontFamily.c_str(),fontStyle,size);
	if (!font) // ���� ���� ��
		return nullptr;

	// ĳ�ÿ� ����
	ResourceEntry entry;
	entry.data = font;
	entry.type = typeid(IRenderFont);
	m_resources[hashString] = entry;

	return font.get();
}


