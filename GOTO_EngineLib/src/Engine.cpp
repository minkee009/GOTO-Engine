#include "Engine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ObjectDestructionManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "BehaviourManager.h"
#ifdef _OS_WINDOWS
#include "WICHelper.h"
#include "DWriteHelper.h"
#include "WinAPIWindow.h"
#endif
using namespace GOTOEngine;

bool Engine::Initialize(int width, int height, const wchar_t* title)
{
	//������ ���� (OS�� �ʱ�ȭ)
	IWindow* window = nullptr;

#ifdef _OS_WINDOWS
	HRESULT hr;

	hr = CoInitialize(nullptr);

	if (!SUCCEEDED(hr))
		return false;

    window = new WinAPIWindow();
#endif

    if (!window->Create(width, height, title))
    {
        delete window;
        return false;
    }

    m_window = window;

	//�Ŵ��� �ʱ�ȭ
#ifdef _OS_WINDOWS
	InputManager::Get()->Startup(static_cast<HWND>(m_window->GetNativeHandle()));
	TimeManager::Get()->StartUp();
#endif
	SceneManager::Get()->StartUp();
    RenderManager::Get()->StartUp(static_cast<IWindow*>(m_window));
	ResourceManager::Get()->StartUp();
	ObjectDestructionManager::Get()->StartUp();

	//���� �ʱ�ȭ
#ifdef _OS_WINDOWS
	WICHelper::StartUp();
	DWriteHelper::StartUp();
#endif

	return true;
}

void Engine::Run()
{
	m_isRunning = true;
	while (m_isRunning)
	{
		m_window->PollEvents();
		if (m_window->ShouldClose())
		{
			m_isRunning = false;
			break;
		}
		ProcessFrame();
	}
}

void Engine::ProcessFrame()
{
	//�ھ� ������Ʈ
	InputManager::Get()->Update();
	TimeManager::Get()->Update();
	bool changedScene = SceneManager::Get()->CheckSceneChange();
	if (changedScene)
	{
		ObjectDestructionManager::Get()->Update();
		BehaviourManager::Get()->DisableBehaviours();
		ObjectDestructionManager::Get()->Clear();
		BehaviourManager::Get()->BroadCastBehaviourMessage("OnSceneLoaded");
	}

	//Behaviour �ʱ�ȭ �޽��� (�ʿ��� ��ü�� ����)
	//BehaviourManager::Get()->BroadCastBehaviourMessage("Awake");
	//BehaviourManager::Get()->BroadCastBehaviourMessage("OnEnable");
	//BehaviourManager::Get()->BroadCastBehaviourMessage("Start");
	BehaviourManager::Get()->InitializeBehaviours();

	//Behaviour ���� (�ʿ��)
	BehaviourManager::Get()->CheckAndSortBehaviours();

	//���� ������Ʈ
	static float accumulator = 0.0f;
	accumulator += TimeManager::Get()->GetDeltaTime();

	if (accumulator > TimeManager::Get()->GetMaximumAllowedTimestep())
		accumulator = TimeManager::Get()->GetMaximumAllowedTimestep();

	//GetTime -> �����ð� ��ȯ���� ����
	TimeManager::Get()->SetExecutionContext(TimeManager::ExcutionContext::Fixed);
	float fixedDelta = TimeManager::Get()->GetFixedDeltaTime();

	while (accumulator >= fixedDelta)
	{
		//���� ����ƽ ����
		accumulator -= fixedDelta;
		TimeManager::Get()->FixedUpdate();
		BehaviourManager::Get()->BroadCastBehaviourMessage("FixedUpdate");
	}

	//GetTime -> �Ϲݽð� ��ȯ���� ����
	TimeManager::Get()->SetExecutionContext(TimeManager::ExcutionContext::Normal);

	//������Ʈ
	BehaviourManager::Get()->BroadCastBehaviourMessage("Update");
	BehaviourManager::Get()->BroadCastBehaviourMessage("LateUpdate");

	//����
	RenderManager::Get()->Clear();
	//RenderManager::Get()->Render();
	RenderManager::Get()->SwapBuffer();

	//���� �� ��ó��
	//ObjectDestructionManager�� ȣ�� ������ �����ϴ� ������� ����
	//BehaviourManager::Get()->BroadCastBehaviourMessage("OnDisable"); 
	//BehaviourManager::Get()->BroadCastBehaviourMessage("OnDestroy");
	ObjectDestructionManager::Get()->Update();
	BehaviourManager::Get()->DisableBehaviours();
	ObjectDestructionManager::Get()->Clear();
}

void Engine::Quit()
{
	m_isRunning = false;
}

void Engine::Shutdown()
{
	InputManager::Get()->Shutdown();
	TimeManager::Get()->Shutdown();
	ResourceManager::Get()->ShutDown();
	SceneManager::Get()->ShutDown();
	ObjectDestructionManager::Get()->Update();
	BehaviourManager::Get()->DisableBehaviours();
	ObjectDestructionManager::Get()->Clear();

	RenderManager::Get()->ShutDown();

#ifdef _OS_WINDOWS
	WICHelper::ShutDown();
	DWriteHelper::ShutDown();
#endif

	if (m_window)
	{
		delete m_window;
		m_window = nullptr;
	}

#ifdef _OS_WINDOWS
	CoUninitialize();
#endif
}
