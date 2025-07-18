#pragma once
#include <Scene.h>
#include <string>
#include <memory>

using namespace GOTOEngine;
class TitleScene : public Scene
{
public:
	TitleScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Initialize() override;

	~TitleScene();
};