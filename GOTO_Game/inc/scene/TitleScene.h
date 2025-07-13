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
public:
	TitleScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Init() override;

	~TitleScene();
};