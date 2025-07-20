#include "ResourceManager.h"
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

