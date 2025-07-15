// �ܼ��� ����� �������� �����մϴ�.

// �⺻ ���̺귯��
#include <iostream>
#include <Windows.h>

// ���� �� �Ŵ���
#include "Engine.h"
#include "SceneManager.h"

// ��
#include "TitleScene.h"
#include "SampleScene.h"

void StartEngineLoop()
{
	ENGINE_INIT(1024, 768, L"D2D Game");

	//�� �Ҵ� �� �߰�
	SCENE_ADD_SCENE<TitleScene>(L"Title");
	SCENE_ADD_SCENE<SampleScene>(L"SampleScene");

	//ù ������ ��ȯ
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