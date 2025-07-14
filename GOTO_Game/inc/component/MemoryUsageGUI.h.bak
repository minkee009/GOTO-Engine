#pragma once
#include <ScriptBehaviour.h>
#include <RenderManager.h>
#include <D2DRenderAPI.h>

namespace GOTOEngine
{
	class MemoryUsageGUI : public ScriptBehaviour
	{
	private:
		D2DRenderAPI* m_api;

		void Awake()
		{
			m_api = dynamic_cast<D2DRenderAPI*>(RenderManager::Get()->GetRenderAPI());
		}

		void OnGUI()
		{
			auto winHeight = RenderManager::Get()->GetWindow()->GetHeight();
			auto winWidth = RenderManager::Get()->GetWindow()->GetWidth();

		 	auto status = m_api->CollectMemoryUsage();

			auto vramUsage = std::wstring(status.vramUsage.begin(), status.vramUsage.end());
			auto dramUsage = std::wstring(status.dramUsage.begin(), status.dramUsage.end());
			auto pagefileUsage = std::wstring(status.pageFileUsage.begin(), status.pageFileUsage.end());

			RenderManager::Get()->DrawString(0, 340, 680, 36, vramUsage.c_str(), nullptr, false, { 255,127,0,255 });
			RenderManager::Get()->DrawString(0, 310, 500, 36, dramUsage.c_str(), nullptr, false, { 255,127,0,255 });
			RenderManager::Get()->DrawString(0, 280, 500, 36, pagefileUsage.c_str(), nullptr, false, { 255,127,0,255 });
		}


	public:
    MemoryUsageGUI()
    {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
    }
	};
}