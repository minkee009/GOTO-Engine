#include "ResourceManager.h"
#include "IRenderImage.h"
#include "D2DImage.h"
#include "D2DFont.h"
#include "Resource.h"


void GOTOEngine::ResourceManager::StartUp()
{
	
}

void GOTOEngine::ResourceManager::ShutDown()
{
	
}

void GOTOEngine::ResourceManager::Clear()
{
	//������ ��� ���ҽ��� �ı�
	for (auto& res : m_resources)
	{
		res->m_refCount = 0;
		Object::DestroyImmediate(res);
	}
	m_resourceTable.clear();
	m_resources.clear();
}

void GOTOEngine::ResourceManager::RegisterResource(Resource* resource)
{
	m_resources.push_back(resource);
}

void GOTOEngine::ResourceManager::UnRegisterResource(Resource* resource)
{
	auto it = std::find(m_resources.begin(), m_resources.end(), resource);
	if (it != m_resources.end()) {
		*it = std::move(m_resources.back()); // ������ ���Ҹ� ���
		m_resources.pop_back();
	}
}

void GOTOEngine::ResourceManager::DestroyUnusedResource()
{
	for (auto& res : m_resources)
	{
		if (res->m_refCount == 0)
		{
			Object::DestroyImmediate(res);
		}
	}
}

//IRenderImage* ResourceManager::LoadStaticImage(const std::wstring& filePath)
//{
//	auto it = m_resources.find(filePath);
//	if (it != m_resources.end())
//	{
//		if (it->second.type == typeid(IRenderImage)) {
//			return static_cast<IRenderImage*>(it->second.data.get());
//		}
//		return nullptr; // Ÿ�� ����ġ - �ٸ� Ÿ������ �̹� �ε�� ���
//	}
//
//	std::shared_ptr<IRenderImage> image = std::make_shared<D2DImage>(filePath);
//	if (!image) // ���� ���� ��
//		return nullptr;
//
//	// ĳ�ÿ� ����
//	ResourceEntry entry;
//	entry.data = image;
//	entry.type = typeid(IRenderImage);
//	m_resources[filePath] = entry;
//
//	return image.get();
//}
//
//IRenderFont* GOTOEngine::ResourceManager::LoadStaticFont(const std::wstring& fontFamily,IRenderFontStyle fontStyle, int size)
//{
//	int styleValue = static_cast<int>(fontStyle);
//
//	std::wstring hashString = fontFamily + L"_" + std::to_wstring(styleValue) +
//		L"_" + std::to_wstring(size);
//
//	auto it = m_resources.find(hashString);
//	if (it != m_resources.end())
//	{
//		if (it->second.type == typeid(IRenderFont)) {
//			return static_cast<IRenderFont*>(it->second.data.get());
//		}
//		return nullptr; // Ÿ�� ����ġ - �ٸ� Ÿ������ �̹� �ε�� ���
//	}
//
//	std::shared_ptr<IRenderFont> font = std::make_shared<D2DFont>(fontFamily.c_str(),fontStyle,size);
//	if (!font) // ���� ���� ��
//		return nullptr;
//
//	// ĳ�ÿ� ����
//	ResourceEntry entry;
//	entry.data = font;
//	entry.type = typeid(IRenderFont);
//	m_resources[hashString] = entry;
//
//	return font.get();
//}


