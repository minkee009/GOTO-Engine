#pragma once
#include "Singleton.h"
#include "Vector2.h"

namespace GOTOEngine
{
	// �������� KeyCode ������ ���� (����)
	enum class KeyCode
	{
		// ���ĺ�
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		// ����
		Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

		// Ư�� Ű
		Escape, Space, Enter, Tab, Backspace, Delete,
		LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
		UpArrow, DownArrow, LeftArrow, RightArrow,

		// ���콺 ��ư
		MouseLeft, MouseRight, MouseMiddle,

		// ��Ÿ (�ʿ信 ���� �߰�)
		Unknown
	};

	class IInputSystem;
	class InputManager : public Singleton<InputManager>
	{
	public:
		InputManager() = default;
		~InputManager() = default;
		void Startup(void* windowHandle);
		void Shutdown();

		void Update();

		Vector2 GetMousePos();
		bool GetKey(KeyCode keyCode);
		bool GetKeyDown(KeyCode keyCode);
		bool GetKeyUp(KeyCode keyCode);
	private:
		IInputSystem* m_inputSystem = nullptr;
	};
}

#define INPUT_GET_KEY GOTOEngine::InputManager::Get()->GetKey
#define INPUT_GET_KEYDOWN GOTOEngine::InputManager::Get()->GetKeyDown
#define INPUT_GET_KEYUP GOTOEngine::InputManager::Get()->GetKeyUp
#define INPUT_GET_MOUSEPOS GOTOEngine::InputManager::Get()->GetMousePos