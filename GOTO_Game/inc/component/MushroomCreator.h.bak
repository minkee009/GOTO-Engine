#pragma once
#include <ScriptBehaviour.h>
#include <InputManager.h>
#include <RenderManager.h>
#include <SpriteRenderer.h>
#include <PlayerRotate.h>
#include <IWindow.h>
#include <random>

namespace GOTOEngine
{
	class MushroomCreator : public ScriptBehaviour
	{
	private:
		void Update()
		{
			auto windowWidth = RenderManager::Get()->GetWindow()->GetWidth();
			auto windowHeight = RenderManager::Get()->GetWindow()->GetHeight();

			if (INPUT_GET_KEYDOWN(KeyCode::Space))
			{
				std::random_device rd;
				std::mt19937 gen(rd());

				// 균등 분포 [0.0, 1.0]
				std::uniform_real_distribution<float> dist(-0.5f, 0.5f);
				
				float randomX = windowWidth * dist(gen);
				float randomY = windowHeight * dist(gen);

				auto monster = new GameObject(L"몬스터");

				//---컴포넌트 추가
				auto monsterSprite = monster->AddComponent<SpriteRenderer>();
				auto monsterRotate = monster->AddComponent<PlayerRotate>();

				std::uniform_real_distribution<float> dist2(145.0f, 245.0f);

				//---Assign
				monsterSprite->SetSprite(Resource::Load<Sprite>(L"../Resources/stand.gif"));
				monsterSprite->SetRenderOrder(1000);
				monsterRotate->rotateSpeed = dist2(gen);

				monster->GetTransform()->SetPosition({ randomX,randomY });
			}
		}

		void OnGUI()
		{
			auto windowWidth = RenderManager::Get()->GetWindow()->GetWidth();
			std::wstring outText = L"SpaceBar로 몬스터 생성";

			RenderManager::Get()->DrawString(windowWidth * 0.5f - 120, 280, 500, 36, outText.c_str(), nullptr, false, { 255,0,0,255 });
		}
	public:
    MushroomCreator()
    {
        REGISTER_BEHAVIOUR_MESSAGE(OnGUI);
        REGISTER_BEHAVIOUR_MESSAGE(Update);
    }
	};
}