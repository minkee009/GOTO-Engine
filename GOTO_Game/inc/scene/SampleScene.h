#pragma once
#include <Scene.h>
#include <string>
#include <memory>

using namespace GOTOEngine;
class SampleScene : public Scene
{
public:
	SampleScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Init() override;
};