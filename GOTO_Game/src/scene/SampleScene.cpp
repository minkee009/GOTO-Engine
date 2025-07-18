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
	//---��������
	RenderManager::Get()->SetVSyncInterval(0);

	//---���ӿ�����Ʈ �ʱ�ȭ
	auto camera = Camera::CreateMainCamera();
	auto BG = new GameObject(L"���");
	auto MemoryAllocator = new GameObject(L"�޸� �Ҵ� ������Ʈ");

	//---������Ʈ �߰�
	BG->AddComponent<SpriteRenderer>();
	MemoryAllocator->AddComponent<MemoryUsageGUI>();
	MemoryAllocator->AddComponent<MushroomCreator>();
	
	//---Assign
	BG->GetComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/giga.png"));
}