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
    RenderManager::Get()->SetVSyncInterval(1);

    //if (!m_font)
    //{
    //	m_font = std::make_unique<D2DFont>(L"Segoe UI", IRenderFontStyle::Bold, 24);
    //}
    //if (!m_image)
    //{
    //	m_image = std::make_unique<D2DImage>(L"../Resources/Mushroom.png",(49/2),(41/2));
    //}

    //if (!m_imageAtlas)
    //{
    //	m_imageAtlas = std::make_unique<D2DImage>(L"../Resources/atk_1.png");
    //}

    //if (!m_animationClip)
    //{
    //	auto animWidth = 171;
    //	auto animHeight = 144;

    //	m_animationClip = AnimationClip::CreateShared({
    //		new D2DImage(m_imageAtlas.get(),0,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),1 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),2 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),3 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),4 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),5 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		new D2DImage(m_imageAtlas.get(),6 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
    //		}, 1 / 8.0f, true);
    //}

    //m_animController.AddAnimationState(L"Attack", *m_animationClip.get());
    //m_animController.Play(L"Attack");

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

//
//void TitleScene::Render()
//{
//	Scene::Render();
//
//	RENDER_DRAW_RECT(0, 0, 150, 150, true, Color{ 255,0, 0, 255 });
//
//	static wchar_t buffer[64];
//	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%.0f fps", (1 / TIME_GET_DELTATIME()));
//
//	RENDER_DRAW_STRING(0, 0, buffer,m_font.get(), true, Color{ 255, 255, 255, 255 });
//	RENDER_DRAW_IMAGE(0, 0, 3.0f, m_image.get());
//
//	RENDER_DRAW_IMAGE(0, 0, 3.0f, &m_animController.CurrentState().GetCurrentKeyFrameImage());
//}

TitleScene::~TitleScene()
{
}
