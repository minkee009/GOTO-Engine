#pragma once
#include "Singleton.h"

namespace GOTOEngine
{
	class IWindow;
	class Engine : public Singleton<Engine>
	{
	public:
		bool Initialize(int width, int height, const wchar_t* title);
		void Run();
		void Quit();  //���� �������� ������ �� ���
		void Shutdown();  // �Ŵ��� ����
		const IWindow* GetWindow() const { return m_window; }
	private:
		IWindow* m_window = nullptr;
		bool m_isRunning = true;
		void ProcessFrame();
	};
}

#define ENGINE_INIT GOTOEngine::Engine::Get()->Initialize
#define ENGINE_RUN GOTOEngine::Engine::Get()->Run
#define ENGINE_QUIT GOTOEngine::Engine::Get()->Quit
#define ENGINE_SHUTDOWN GOTOEngine::Engine::Get()->Shutdown
#define ENGINE_GET_WINDOW GOTOEngine::Engine::Get()->GetWindow