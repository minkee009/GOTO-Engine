#include "TitleScene.h"
#include <RenderManager.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <SceneManager.h>
#include <ResourceManager.h>
#include <D2DFont.h>
#include <Mathf.h>
#include <GameObject.h>
#include <Transform.h>
#include <iostream>
#include "TestComponent.h"
#include "Camera.h"
#include "PlayerMove.h"
#include "PlayerRotate.h"
#include <SpriteRenderer.h>
#include <TextRenderer.h>
#include "CameraMove.h"
#include "RenderInfoDraw.h"
#include "AnimatorController.h"
#include <RigidBody2D.h>
#include <Collider2D.h>

#include <AnimationClip.h>

using namespace GOTOEngine;

void TitleScene::Initialize()
{
    //---엔진 옵션 변경
    //TimeManager::Get()->SetFixedDeltaTime(0.05f);

    //---오브젝트 초기화
    auto mushmom_GO = new GameObject(L"머쉬맘");
    auto slime_GO = new GameObject(L"슬라임");
    auto swallow_GO = new GameObject(L"파란달팽이");
    auto backGround_GO = new GameObject(L"BG");
    auto camera_GO = Camera::CreateMainCamera();
    auto camera2_GO = Camera::CreateMainCamera();
    auto guiDraw_Go = new GameObject(L"GUI");

    //---트랜스 폼 설정
    mushmom_GO->GetTransform()->SetPosition({ 0,0 });
    mushmom_GO->GetTransform()->SetLossyScale({ 2.0f,2.0f });

    slime_GO->GetTransform()->SetParent(mushmom_GO->GetTransform(),false);
    slime_GO->GetTransform()->SetLocalPosition(Vector2{ 175.0f, 50.0f });

    swallow_GO->GetTransform()->SetParent(slime_GO->GetTransform(), false);
    swallow_GO->GetTransform()->SetLocalPosition(Vector2{ 70.0f, 0.0f });

    backGround_GO->GetTransform()->SetLossyScale({ 3.0f,3.0f });
    camera2_GO->GetTransform()->SetParent(swallow_GO->GetTransform(), false);

    //---오브젝트 컴포넌트 추가
    auto spriteRendererBG = backGround_GO->AddComponent<SpriteRenderer>();
    spriteRendererBG->SetSprite(Resource::Load<Sprite>(L"../Resources/Lith_Harbor.png"));
    spriteRendererBG->SetRenderOrder(-1000);

    mushmom_GO->AddComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/stand.gif"));
    mushmom_GO->AddComponent<PlayerRotate>();

    slime_GO->AddComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/slime.png"));
    slime_GO->AddComponent<PlayerRotate>();

    swallow_GO->AddComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/blue.gif"));
    swallow_GO->AddComponent<PlayerRotate>();
    swallow_GO->AddComponent<TextRenderer>()->SetFont(Resource::Load<Font>(L"../Resources/Maplestory Bold.ttf"));
    
    camera_GO->AddComponent<CameraMove>();
    camera2_GO->AddComponent<CameraMove>();

    guiDraw_Go->AddComponent<RenderInfoDraw>();


    //---컴포넌트 어사인
    auto swallowText = swallow_GO->GetComponent<TextRenderer>();
    swallowText->text = L"안녕하세요!";
    swallowText->size = 32;
    swallowText->SetRenderOrder(1500);
    camera_GO->GetComponent<Camera>()->SetRect({ 0,0,1.0f,1.0f });

    auto camera2_cameraComp = camera2_GO->GetComponent<Camera>(); 
    camera2_cameraComp->SetRect({ 0.7f,0.05f,0.25f,0.25f });
    camera2_cameraComp->SetDepth(1000);
    camera2_cameraComp->SetSize(0.5f);
    camera2_GO->GetComponent<CameraMove>()->isSub = true;


    //Don't Destory On Load 씬으로 이동
    Object::DontDestroyOnLoad(mushmom_GO);
}     
