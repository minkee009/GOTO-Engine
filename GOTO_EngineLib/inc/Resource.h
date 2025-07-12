#pragma once
#include "Object.h"
#include "ResourceManager.h"
#include <string>

namespace GOTOEngine
{
	class Resource : public Object
	{
	private:
		friend class ResourceManager;
		friend class Sprite;
	protected:
		virtual void LoadFromFilePath(const std::wstring& filePath) = 0;
		virtual bool IsValidRawData() = 0;
		size_t m_refCount;
		std::wstring m_filePath;
		Resource() : m_refCount(0), m_filePath(L"") { ResourceManager::Get()->RegisterResource(this); }
	    ~Resource() 
		{ 
			auto& resourceTable = ResourceManager::Get()->m_resourceTable;
			auto resourceTypeInfo = resourceTable.find(typeid(*this));
			if (resourceTypeInfo != resourceTable.end())
			{
				auto& resourceTypeTable = resourceTable[typeid(*this)];
				auto resource = resourceTypeTable.find(m_filePath);
				if (resource != resourceTypeTable.end())
				{
					resourceTypeTable.erase(resource);
				}
			}
			ResourceManager::Get()->UnRegisterResource(this); 
			std::cout << "resource unloaded" << this << std::endl;
		}
	public:
		std::wstring GetFilePath() { return m_filePath; }

	public:
		void IncreaseRefCount() { m_refCount++; }
		void DecreaseRefCount() { if(m_refCount != 0) m_refCount--; if (m_refCount == 0) DestroyImmediate(this); }

		template <typename T>
		static T* Load(std::wstring filePath)
		{
			return ResourceManager::Get()->Load<T>(filePath);
		}
	};
}