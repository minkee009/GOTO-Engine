#pragma once
#include <ScriptBehaviour.h>
#include <TimeManager.h>
#include <RenderManager.h>
#include <IWindow.h>
#include <string>

namespace GOTOEngine
{
	class RenderInfoDraw : public ScriptBehaviour
	{
	private:
		float m_rot = 0;
	public:
    RenderInfoDraw()
    {
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
    }
		void OnGUI()
		{
			auto window = RenderManager::Get()->GetWindow();
			RenderManager::Get()->DrawString(0, window->GetHeight() - 540, 680, 540, L"W,A,S,D - 메인 카메라 이동, 방향키 - 서브 카메라 이동", nullptr, false, {255,127,0,255});
		}
	};
}