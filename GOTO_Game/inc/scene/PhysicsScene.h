#pragma once
#include <Scene.h>
#include <string>
#include <memory>

using namespace GOTOEngine;
class PhysicsScene : public Scene
{
public:
	PhysicsScene(const std::wstring& sceneName) : Scene(sceneName) {}
	void Initialize() override;
};