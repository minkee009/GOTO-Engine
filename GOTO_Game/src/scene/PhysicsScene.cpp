#include "PhysicsScene.h"
#include "Camera.h"
#include "Collider2D.h"
#include "RigidBody2D.h"

#include <Animator.h>
#include <AudioClip.h>
#include <AudioSource.h>
#include "PlayerController.h"
#include "PlayerFSMController.h"
#include "TimeManager.h"

void PhysicsScene::Initialize()
{
	Camera::CreateMainCamera();

	auto GroundGO = new GameObject(L"땅바닥");

	GroundGO->AddComponent<Collider2D>()->SetSize({ 3500.0f,50.0f });
	auto GroundRB = GroundGO->AddComponent<RigidBody2D>();
	GroundRB->SetPosition({ 0.0f,-250.0f });
	GroundRB->SetRotation(-15);
	GroundRB->SetMass(FLT_MAX);

	int boxIndex = 0;
	int pyramidHeight = 5;
	float boxWidth = 50.0f;
	float boxHeight = 50.f;

	for (int row = 0; row < pyramidHeight; ++row)
	{
		int boxCount = pyramidHeight - row;

		// 현재 줄의 박스들을 중심 정렬하기 위해 시작 X 계산
		float startX = -(boxCount - 1) * boxWidth * 0.5f;

		// Y 위치는 층에 따라 위로 올라감
		float y = row * boxHeight;

		for (int col = 0; col < boxCount; ++col)
		{
			float x = startX + col * boxWidth;

			auto BoxGO = new GameObject(L"Box_" + std::to_wstring(boxIndex++));
			BoxGO->AddComponent<Collider2D>();
			auto BoxRB = BoxGO->AddComponent<RigidBody2D>();

			BoxRB->SetPosition({ x, y });
		}
	}

	auto player = new GameObject(L"플레이어");

	player->AddComponent<SpriteRenderer>()->AddComponent<SpriteRenderer>();
	player->AddComponent<Animator>()->SetAnimatorController(Resource::Load<AnimatorController>(L"../Resources/Animation/controller/KnightAnimator_AnimController.json"));

	player->AddComponent<PlayerFSMController>();

	player->GetTransform()->SetLocalScale({ 10.0f,10.0f });

	auto audioSource = player->AddComponent<AudioSource>();
	audioSource->SetPlayOnAwake(true);

	audioSource->SetClip(Resource::Load<AudioClip>(L"../Resources/boss3.mp3"));
}
