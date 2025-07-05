#include "TimeManager.h"

using namespace GOTOEngine;
float GOTOEngine::TimeManager::GetDeltaTime() const
{
    switch (m_executionContext)
    {
    case ExcutionContext::Normal:
        return m_deltaTime; // �Ϲ� ������Ʈ������ m_deltaTime�� ��ȯ
	case ExcutionContext::Fixed:
		return m_fixedDeltaTime; // ���� ������Ʈ������ m_fixedDeltaTime�� ��ȯ
    default:
        return 0.0f;
    }
}
float GOTOEngine::TimeManager::GetTime() const
{
    switch (m_executionContext)
    {
    case ExcutionContext::Normal:
        return m_totalTime; // �Ϲ� ������Ʈ������ m_deltaTime�� ��ȯ
    case ExcutionContext::Fixed:
        return m_fixedTime; // ���� ������Ʈ������ m_fixedDeltaTime�� ��ȯ
    default:
        return 0.0f;
    }
}
void GOTOEngine::TimeManager::StartUp()
{
    // �ʱ� �ð� ���
    m_initTime = std::chrono::high_resolution_clock::now();
    m_prevTime = m_initTime;
}

void GOTOEngine::TimeManager::Shutdown()
{

}

void GOTOEngine::TimeManager::Update()
{
    // ���� �ð� ���
    m_currentTime = std::chrono::high_resolution_clock::now();

    // ���� �����Ӱ��� �ð� ���� (std::chrono::duration���� ��ȯ)
    auto delta = m_currentTime - m_prevTime;
    m_unscaledDeltaTime = std::chrono::duration<float>(delta).count();

    // timeScale�� ���� deltaTime
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    // ���� ���� ���� �ð�
    std::chrono::duration<float> unscaledTotalDuration = m_currentTime - m_initTime;
    m_unscaledTotalTime = unscaledTotalDuration.count(); // �� ���� float���� ��ȯ

    m_totalTime = m_unscaledTotalTime * m_timeScale;

    // ���� �ð� ����
    m_prevTime = m_currentTime;
}

void GOTOEngine::TimeManager::FixedUpdate()
{
	m_fixedTime += m_fixedDeltaTime;
}

