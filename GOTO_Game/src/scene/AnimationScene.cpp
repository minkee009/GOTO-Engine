#include "AnimationScene.h"
#include <Animator.h>
#include "PlayerController.h"
#include "Camera.h"

void AnimationScene::Initialize()
{
	auto camera = Camera::CreateMainCamera()->GetComponent<Camera>();
	auto player = new GameObject(L"플레이어");
	camera->SetBackGroundColor({ 17,150,193,255 });

	player->AddComponent<SpriteRenderer>()->AddComponent<SpriteRenderer>();
	player->AddComponent<Animator>()->SetAnimatorController(Resource::Load<AnimatorController>(L"../Resources/Animation/controller/KnightAnimator_AnimController.json"));
	
	player->AddComponent<PlayerController>();

	player->GetTransform()->SetLocalScale({ 10.0f,10.0f });
}
