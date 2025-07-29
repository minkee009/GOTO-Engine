#pragma once
#include <Singleton.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <Color.h>
#include <wincodec.h>
#include <typeindex>
#include "Object.h"


namespace GOTOEngine
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	private:
		friend class Engine;
		friend class Resource;

		std::vector<Resource*> m_resources;
		std::unordered_map < std::type_index, std::unordered_map <std::wstring, Resource*> > m_resourceTable; // map < typename, map < filePath, Resource* > >

		void StartUp();
		void ShutDown();
		void Clear();

		void RegisterResource(Resource* resource);
		void UnRegisterResource(Resource* resource);

		void DestroyUnusedResource();

		template <typename T>
		T* Load(std::wstring filePath)
		{
			//T�� �ڽ��� ��ӹ޴� Ŭ������
			static_assert(std::is_base_of<Resource, T>::value, "T must be derived from Resource");

			std::type_index typeIdx = typeid(T); // ��û�� Ÿ�� T�� type_index

			auto sss = m_resourceTable;

			//���ҽ� �Ŵ������� ���ҽ����̺� ���� �˻�
			//--- 1. Ÿ�� �˻�
			auto typeMapIt = m_resourceTable.find(typeIdx);
			if (typeMapIt != m_resourceTable.end())
			{
				//--- 2. ���ϰ�� �˻�
				auto& filePathMap = typeMapIt->second; 
				auto resourceIt = filePathMap.find(filePath);

				if (resourceIt != filePathMap.end())
				{
					//--- 3. ���ҽ� �˻�: ���� ��η� ���ҽ��� �ִ� ���
					// ��۷� ������ Ȯ��
					if (Object::IsValidObject(resourceIt->second) 
						&& !resourceIt->second->IsDestroyed())
					{
						// ĳ������ �������� Ȯ�� (dynamic_cast ���, static_cast�� �������� ���߿� üũ�ϱ�) 
						T* existingResource = dynamic_cast<T*>(resourceIt->second);
						if (existingResource) {
							//existingResource->IncreaseRefCount();
							return existingResource;
						}
					}
					// ��ȿ���� ���� ���ҽ��� �Ǵ�, ���ҽ� ���̺��� ���� ����
					filePathMap.erase(resourceIt);
				}
			}

			//���� ���
			T* newResource = new T();
			newResource->m_filePath = filePath; // ���� ��� ����
			newResource->LoadFromFilePath(filePath); // ���Ͽ��� ������ �ε�

			if (!newResource->IsValidData())
			{
				Object::DestroyImmediate(newResource);
				return nullptr;
			}

			// ResourceManager�� ���ο� ���ҽ� ���
			m_resourceTable[typeIdx].insert_or_assign(filePath, newResource);

			return newResource;
		}
	};
}
