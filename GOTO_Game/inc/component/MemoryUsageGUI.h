#pragma once
#include <ScriptBehaviour.h>
#include <RenderManager.h>
#include <TimeManager.h>
#include <IRenderAPI.h>
#include <WStringHelper.h>

namespace GOTOEngine
{
	class MemoryUsageGUI : public ScriptBehaviour
	{
	private:
		RenderAPIMemoryStatus m_status;

		void FixedUpdate()
		{
			m_status = RenderManager::Get()->CollectMemoryUsage();
		}

		void OnGUI()
		{
			auto winHeight = RenderManager::Get()->GetWindow()->GetHeight();
			auto winWidth = RenderManager::Get()->GetWindow()->GetWidth();

			auto vramUsage = L"VRam - " + std::wstring(m_status.vramUsage.begin(), m_status.vramUsage.end());
			auto dramUsage = L"DRam - " + std::wstring(m_status.dramUsage.begin(), m_status.dramUsage.end());
			auto pagefileUsage = L"PageFile - " + std::wstring(m_status.pageFileUsage.begin(), m_status.pageFileUsage.end());

			RenderManager::Get()->DrawString(vramUsage.c_str(), Rect{0, 340, 680, 36}, Color{ 0,255,0,255 });
			RenderManager::Get()->DrawString(dramUsage.c_str(), Rect{ 0, 310, 500, 36 }, Color{ 0,255,0,255 });
			RenderManager::Get()->DrawString(pagefileUsage.c_str(), Rect{ 0, 280, 500, 36, }, Color{ 0,255,0,255 });

			//fps Ãâ·Â
			std::wstring fpsText = std::to_wstring(1/TIME_GET_DELTATIME());
			RenderManager::Get()->DrawString(fpsText.c_str(), Rect{ 0, (float)winHeight - 540, 680, 540 }, Color{ 0,0,255,255 });
		}


	public:
    MemoryUsageGUI()
    {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
    }
	};
}