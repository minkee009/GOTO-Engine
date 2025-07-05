#include "Engine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ObjectDestructionManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
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
	SceneManager::Get()->Update();

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

		SceneManager::Get()->FixedUpdate();
		//BehaviourManager::Get()->FixedUpdate();
	}

	//GetTime -> 일반시간 반환으로 변경
	TimeManager::Get()->SetExecutionContext(TimeManager::ExcutionContext::Normal);

	//업데이트
	//BehaviourManager::Get()->Update();
	//BehaviourManager::Get()->LateUpdate();
	ObjectDestructionManager::Get()->Update();

	//렌더
	RenderManager::Get()->Clear();
	//RenderManager::Get()->Render();
	RenderManager::Get()->SwapBuffer();
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
	ObjectDestructionManager::Get()->ShutDown();
	SceneManager::Get()->ShutDown();

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
