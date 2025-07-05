#include "TitleScene.h"
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

using namespace GOTOEngine;

GameObject* g_test01 = nullptr;
GameObject* g_test02 = nullptr;

void TitleScene::Enter()
{
	RenderManager::Get()->SetVSyncInterval(0);

	if (!m_font)
	{
		m_font = std::make_unique<D2DFont>(L"Segoe UI", IRenderFontStyle::Bold, 24);
	}
	if (!m_image)
	{
		m_image = std::make_unique<D2DImage>(L"../Resources/Mushroom.png",(49/2),(41/2));
	}

	if (!m_imageAtlas)
	{
		m_imageAtlas = std::make_unique<D2DImage>(L"../Resources/atk_1.png");
	}

	if (!m_animationClip)
	{
		auto animWidth = 171;
		auto animHeight = 144;

		m_animationClip = AnimationClip::CreateShared({
			new D2DImage(m_imageAtlas.get(),0,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),1 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),2 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),3 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),4 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),5 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			new D2DImage(m_imageAtlas.get(),6 * animWidth,0,animWidth,animHeight,animWidth / 2, animHeight / 2),
			}, 1 / 8.0f, true);
	}

	m_animController.AddAnimationState(L"Attack", *m_animationClip.get());
	m_animController.Play(L"Attack");

	g_test01 = new GameObject(L"Player");
	g_test02 = new GameObject(L"Child");

	g_test02->GetTransform()->SetParent(g_test01->GetTransform());

	g_test02->GetTransform()->SetPosition(Vector3{ 2.5f,0,0 });
}

void TitleScene::Update()
{
	Scene::Update();
	m_animController.Update(TIME_GET_DELTATIME());

	static Vector3 angle{ 0,0,0 };


	if (Object::IsValidObject(g_test01))
	{
		angle += (Vector3{0,0, 125.0f } *TIME_GET_DELTATIME());
		g_test01->GetTransform()->GetTransform()->SetEulerAngle(angle);
		std::cout << g_test01->GetTransform()->GetLocalEulerAngle().z << std::endl;

		Object::Destroy(g_test01, 5.0f); // 5초 후에 파괴 예약
	}

	if (Object::IsValidObject(g_test02))
	{
		std::cout << "child : " << g_test02->GetTransform()->GetPosition().x << std::endl;
	}
}
void TitleScene::FixedUpdate()
{


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

void TitleScene::Exit()
{
	Scene::Exit();
}

TitleScene::~TitleScene()
{
}
