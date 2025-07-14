#include "SampleScene.h"
#include <GameObject.h>
#include <Camera.h>
#include <SpriteRenderer.h>
#include "PlayerHealth.h"
#include "MonsterAttack.h"
#include "PlayerGUI.h"

void SampleScene::Init()
{
	//---���ӿ�����Ʈ �ʱ�ȭ
	auto camera = Camera::CreateMainCamera();
	auto BG = new GameObject(L"���");
	auto playerGUI = new GameObject(L"GUI");
	auto player = new GameObject(L"�÷��̾�");
	auto monster = new GameObject(L"����");

	//---������Ʈ �߰�
	BG->AddComponent<SpriteRenderer>();
	playerGUI->AddComponent<PlayerGUI>();
	player->AddComponent<PlayerHealth>();
	player->AddComponent<SpriteRenderer>();
	monster->AddComponent<MonsterAttack>();
	
	//---Assign
	BG->GetComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/giga.png"));
	player->GetComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/stand.gif"));
	player->GetComponent<SpriteRenderer>()->SetRenderOrder(1000);
	playerGUI->GetComponent<PlayerGUI>()->health = player->GetComponent<PlayerHealth>();
}