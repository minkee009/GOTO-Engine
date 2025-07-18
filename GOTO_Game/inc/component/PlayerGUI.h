#pragma once
#include <ScriptBehaviour.h>
#include <RenderManager.h>
#include <IWindow.h>
#include "PlayerHealth.h"

namespace GOTOEngine
{
	class PlayerHealth;
	class PlayerGUI : public ScriptBehaviour
	{
	private:
		void OnGUI()
		{
			auto winHeight = RenderManager::Get()->GetWindow()->GetHeight();
			auto winWidth = RenderManager::Get()->GetWindow()->GetWidth();

			std::wstring hpText = L" �׾��� ";

			if (IsValidObject(health) && !health->IsDestroyed())
			{
				int hp = health->GetHP();
				hpText = std::to_wstring(hp);
			}

			std::wstring outText = L" ���� �÷��̾��� ü�� : " + hpText;
			
			RenderManager::Get()->DrawString(outText.c_str(), { 0, (float)winHeight - 540, 680, 540 }, { 255, 127, 0, 255 });

			if (IsValidObject(health) && !health->IsDestroyed())
			{
				std::wstring outText2 = L"SpaceBar�� ȥ���ֱ�";

				RenderManager::Get()->DrawString(outText2.c_str(), { winWidth * 0.5f - 120, 280, 500, 36 }, { 255,0,0,255 });
			}
		}
	public:
    PlayerGUI()
    {
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
    }
		PlayerHealth* health;
	};
}