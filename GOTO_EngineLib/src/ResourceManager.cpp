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
	//강제로 모든 리소스를 파괴
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
		*it = std::move(m_resources.back()); // 마지막 원소를 덮어씀
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
//		return nullptr; // 타입 불일치 - 다른 타입으로 이미 로드된 경우
//	}
//
//	std::shared_ptr<IRenderImage> image = std::make_shared<D2DImage>(filePath);
//	if (!image) // 생성 실패 시
//		return nullptr;
//
//	// 캐시에 저장
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
//		return nullptr; // 타입 불일치 - 다른 타입으로 이미 로드된 경우
//	}
//
//	std::shared_ptr<IRenderFont> font = std::make_shared<D2DFont>(fontFamily.c_str(),fontStyle,size);
//	if (!font) // 생성 실패 시
//		return nullptr;
//
//	// 캐시에 저장
//	ResourceEntry entry;
//	entry.data = font;
//	entry.type = typeid(IRenderFont);
//	m_resources[hashString] = entry;
//
//	return font.get();
//}


