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
	class ResourceManager : public Singleton<ResourceManager>
	{
	//public:
		//IRenderImage* LoadStaticImage(const std::wstring& filePath);
		//IRenderFont* LoadStaticFont(const std::wstring& fontFamily,IRenderFontStyle fontStyle,int size);

	private:
		friend class Engine;
		friend class Resource;
		//std::unordered_map<std::wstring, ResourceEntry> m_resources;

		std::vector<Resource*> m_resources;
		std::unordered_map < std::type_index, std::unordered_map <std::wstring, Resource*> > m_resourceTable; // map < typename, map < filePath, Resource* > >

		void StartUp();
		void ShutDown();

		void RegisterResource(Resource* resource);
		void UnRegisterResource(Resource* resource);

		void UnloadUnusedResource();

		template <typename T>
		T* Load(std::wstring filePath)
		{
			//T�� �ڽ��� ��ӹ޴� Ŭ������
			static_assert(std::is_base_of<Resource, T>::value, "T must be derived from Resource");

			std::type_index typeIdx = typeid(T); // ��û�� Ÿ�� T�� type_index

			//���ҽ� �Ŵ������� ���ҽ����̺� ���� �˻�
			//--- 1. Ÿ�� �˻�
			auto typeMapIt = m_resourceTable.find(typeIdx);
			if (typeMapIt != m_resourceTable.end())
			{
				//--- 2. ���ϰ�� �˻�
				auto& filePathMap = typeMapIt->second; // std::unordered_map <std::wstring, Resource*>
				auto resourceIt = filePathMap.find(filePath);

				if (resourceIt != filePathMap.end())
				{
					//--- 3. ���ҽ� �˻�: ���� ��η� ���ҽ��� �ִ� ���
					// ĳ������ �������� Ȯ�� (dynamic_cast ���, static_cast�� �������� ���߿� üũ�ϱ�) 
					T* existingResource = dynamic_cast<T*>(resourceIt->second);
					if (existingResource) {
						existingResource->IncreaseRefCount();
						return existingResource;
					}
				}
			}

			//���� ���
			T* newResource = new T();
			newResource->m_filePath = filePath; // ���� ��� ����
			newResource->CreateRawDataFromFilePath(filePath); // ���Ͽ��� ������ �ε�

			if (!newResource->IsValidRawData())
			{
				DestroyImmediate(newResource);
				return nullptr;
			}

			// ResourceManager�� ���ο� ���ҽ� ���
			m_resourceTable[typeIdx].insert_or_assign(filePath, newResource);

			return newResource;
		}
	};
}
