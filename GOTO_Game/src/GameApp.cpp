// 콘솔을 겸용한 진입점을 정의합니다.

// 기본 라이브러리
#include <iostream>
#include <Windows.h>

// 엔진 및 매니저
#include "Engine.h"
#include "SceneManager.h"

// 씬
#include "TitleScene.h"
#include "SampleScene.h"

void StartEngineLoop()
{
	ENGINE_INIT(1024, 768, L"D2D Game");

	//씬 할당 및 추가
	SCENE_ADD_SCENE<TitleScene>(L"Title");
	SCENE_ADD_SCENE<SampleScene>(L"SampleScene");

	//첫 씬으로 변환
	SCENE_CHANGE_SCENE(L"Title");

	ENGINE_RUN();
	ENGINE_SHUTDOWN();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	StartEngineLoop();
	return 0;
}

int main()
{
	StartEngineLoop();
	return 0;
}