#include "SampleScene.h"
#include <GameObject.h>
#include <Camera.h>
#include <SpriteRenderer.h>
#include "PlayerHealth.h"
#include "MonsterAttack.h"
#include "PlayerGUI.h"
#include "MemoryUsageGUI.h"
#include "MushroomCreator.h"

void SampleScene::Initialize()
{
	//---엔진세팅
	RenderManager::Get()->SetVSyncInterval(0);

	//---게임오브젝트 초기화
	auto camera = Camera::CreateMainCamera();
	auto BG = new GameObject(L"배경");
	auto MemoryAllocator = new GameObject(L"메모리 할당 오브젝트");

	//---컴포넌트 추가
	BG->AddComponent<SpriteRenderer>();
	MemoryAllocator->AddComponent<MemoryUsageGUI>();
	MemoryAllocator->AddComponent<MushroomCreator>();
	
	//---Assign
	BG->GetComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/giga.png"));
}