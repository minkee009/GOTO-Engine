#pragma once
#include <Scene.h>

using namespace GOTOEngine;
class AnimationScene : public Scene
{
public:
	AnimationScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Initialize() override;
};