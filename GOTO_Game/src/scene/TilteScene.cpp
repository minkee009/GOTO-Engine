#include "scene/TitleScene.h"
#include <RenderManager.h>
#include <InputManager.h>
#include <TimeManager.h>
#include <SceneManager.h>
#include <ResourceManager.h>
#include <D2DFont.h>
#include <D2DImage.h>
#include <Mathf.h>
#include <GameObject.h>
#include <Transform.h>
#include <iostream>
#include "TestComponent.h"
#include "Camera.h"
#include "PlayerMove.h"
#include "PlayerRotate.h"
#include <SpriteRenderer.h>

using namespace GOTOEngine;

void TitleScene::Init()
{
    RenderManager::Get()->SetVSyncInterval(0);

    //if (!m_font)
    //{
    //	m_font = std::make_unique<D2DFont>(L"Segoe UI", IRenderFontStyle::Bold, 24);
    //}

    //---오브젝트 초기화
    auto mushroom01_GO = new GameObject(L"Player");
    auto mushroom02_GO = new GameObject(L"Child");
    auto camera_GO = Camera::CreateMainCamera();


    //---트랜스 폼 설정
    mushroom01_GO->GetTransform()->SetPosition({ 0,0 });
    //mushroom01_GO->GetTransform()->SetLossyScale({ 5.0f,5.0f });
 
    mushroom02_GO->GetTransform()->SetParent(mushroom01_GO->GetTransform());
    mushroom02_GO->GetTransform()->SetLocalPosition(Vector2{ 50.0f,50.0f });


    //---오브젝트 컴포넌트 추가
    mushroom01_GO->AddComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/Mushroom.png"));

    mushroom02_GO->AddComponent<SpriteRenderer>()->SetSprite(Resource::Load<Sprite>(L"../Resources/Mushroom.png"));
    mushroom02_GO->AddComponent<PlayerRotate>();
    
    camera_GO->AddComponent<PlayerMove>();

    auto testComp01 = mushroom01_GO->AddComponent<TestComponent>();

    //---컴포넌트 어사인
    testComp01->otherGameObject = mushroom02_GO;


    //---추가 코드
    Object::Destroy(mushroom02_GO, 15.0f);
}     

TitleScene::~TitleScene()
{
}
