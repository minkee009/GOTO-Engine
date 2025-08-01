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
	//윈도우 생성 (OS별 초기화)
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

	//매니저 초기화
#ifdef _OS_WINDOWS
	InputManager::Get()->Startup(static_cast<HWND>(m_window->GetNativeHandle()));
	TimeManager::Get()->StartUp();
#endif
	SceneManager::Get()->StartUp();
    RenderManager::Get()->StartUp(static_cast<IWindow*>(m_window));
	ResourceManager::Get()->StartUp();
	ObjectDestructionManager::Get()->StartUp();

	//헬퍼 초기화
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
	//코어 업데이트
	InputManager::Get()->Update();
	TimeManager::Get()->Update();
	bool changedScene = SceneManager::Get()->CheckSceneChange();
	if (changedScene)
	{
		ObjectDestructionManager::Get()->Update();
		BehaviourManager::Get()->DisableBehaviours();
		ObjectDestructionManager::Get()->Clear();
	}

	//Behaviour 초기화 메시지 (필요한 객체에 한해)
	//BehaviourManager::Get()->BroadCastBehaviourMessage("Awake");
	//BehaviourManager::Get()->BroadCastBehaviourMessage("OnEnable");
	//BehaviourManager::Get()->BroadCastBehaviourMessage("Start");
	BehaviourManager::Get()->InitializeBehaviours();

	//Behaviour 정렬 (필요시)
	BehaviourManager::Get()->CheckAndSortBehaviours();

	//고정 업데이트
	static float accumulator = 0.0f;
	accumulator += TimeManager::Get()->GetDeltaTime();

	if (accumulator > TimeManager::Get()->GetMaximumAllowedTimestep())
		accumulator = TimeManager::Get()->GetMaximumAllowedTimestep();

	//GetTime -> 고정시간 반환으로 변경
	TimeManager::Get()->SetExecutionContext(TimeManager::ExcutionContext::Fixed);
	float fixedDelta = TimeManager::Get()->GetFixedDeltaTime();

	while (accumulator >= fixedDelta)
	{
		//실제 고정틱 실행
		accumulator -= fixedDelta;
		TimeManager::Get()->FixedUpdate();
		BehaviourManager::Get()->BroadCastBehaviourMessage("FixedUpdate");
	}

	//GetTime -> 일반시간 반환으로 변경
	TimeManager::Get()->SetExecutionContext(TimeManager::ExcutionContext::Normal);

	//업데이트
	BehaviourManager::Get()->BroadCastBehaviourMessage("Update");
	BehaviourManager::Get()->BroadCastBehaviourMessage("LateUpdate");

	//렌더
	RenderManager::Get()->Render();

	//렌더 후 후처리
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
	BehaviourManager::Get()->ShutDown();
	ObjectDestructionManager::Get()->ShutDown();

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
