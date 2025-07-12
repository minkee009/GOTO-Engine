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
		void IncreaseRefCount() { m_refCount++; }
		void DecreaseRefCount() { m_refCount--; if (m_refCount == 0) DestroyImmediate(this); }
	protected:
		virtual void CreateRawDataFromFilePath(const std::wstring& filePath) = 0;
		virtual bool IsValidRawData() = 0;
		size_t m_refCount;
		std::wstring m_filePath;
		Resource() : m_refCount(0), m_filePath(L"") { ResourceManager::Get()->RegisterResource(this); }
		virtual ~Resource() 
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
		}
	public:
		std::wstring GetFilePath() { return m_filePath; }

		template <typename T>
		static T* Load(std::wstring filePath)
		{
			ResourceManager::Get()->Load<T>(filePath);
		}
	};
}