#pragma once
#include <Scene.h>
#include <string>
#include <memory>
#include <IRenderFont.h>
#include <IRenderImage.h>
#include "AnimationController.h"

using namespace GOTOEngine;
class TitleScene : public Scene
{
private:
	std::unique_ptr<GOTOEngine::IRenderFont> m_font;
	std::unique_ptr<GOTOEngine::IRenderImage> m_image;

	std::unique_ptr<GOTOEngine::IRenderImage> m_imageAtlas;
	std::shared_ptr<const AnimationClip> m_animationClip;
	AnimationController m_animController;
public:
	TitleScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Enter();
	void Exit();

	//삭제할 코드------------//
	void Update();			//
	void FixedUpdate();		//
	//----------------------//

	~TitleScene();
};