#include "InputManager.h"
#ifdef _OS_WINDOWS
#include "WinAPIInputSystem.h"
#endif

using namespace GOTOEngine;

void InputManager::Startup(void* windowHandle)
{
#ifdef _OS_WINDOWS
	m_inputSystem = new WinAPIInputSystem();
	m_inputSystem->Startup(windowHandle);
#endif
}

void InputManager::Shutdown()
{
	if (m_inputSystem)
	{
		delete m_inputSystem;
		m_inputSystem = nullptr;
	}
}

void InputManager::Update()
{
	if (m_inputSystem)
		m_inputSystem->Update();
}

Vector2 InputManager::GetMousePos()
{
	if (m_inputSystem)
		return m_inputSystem->GetMousePos();

	return Vector2::Zero();
}

bool InputManager::GetKey(KeyCode keyCode)
{
	if (m_inputSystem)
		return m_inputSystem->GetKey(keyCode);

	return false;
}

bool InputManager::GetKeyDown(KeyCode keyCode)
{
	if (m_inputSystem)
		return m_inputSystem->GetKeyDown(keyCode);

	return false;
}

bool InputManager::GetKeyUp(KeyCode keyCode)
{
	if (m_inputSystem)
		return m_inputSystem->GetKeyUp(keyCode);

	return false;
}


