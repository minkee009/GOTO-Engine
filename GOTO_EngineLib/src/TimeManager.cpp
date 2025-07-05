#include "TimeManager.h"

using namespace GOTOEngine;
float GOTOEngine::TimeManager::GetDeltaTime() const
{
    switch (m_executionContext)
    {
    case ExcutionContext::Normal:
        return m_deltaTime; // 일반 업데이트에서는 m_deltaTime을 반환
	case ExcutionContext::Fixed:
		return m_fixedDeltaTime; // 고정 업데이트에서는 m_fixedDeltaTime을 반환
    default:
        return 0.0f;
    }
}
float GOTOEngine::TimeManager::GetTime() const
{
    switch (m_executionContext)
    {
    case ExcutionContext::Normal:
        return m_totalTime; // 일반 업데이트에서는 m_deltaTime을 반환
    case ExcutionContext::Fixed:
        return m_fixedTime; // 고정 업데이트에서는 m_fixedDeltaTime을 반환
    default:
        return 0.0f;
    }
}
void GOTOEngine::TimeManager::StartUp()
{
    // 초기 시간 기록
    m_initTime = std::chrono::high_resolution_clock::now();
    m_prevTime = m_initTime;
}

void GOTOEngine::TimeManager::Shutdown()
{

}

void GOTOEngine::TimeManager::Update()
{
    // 현재 시간 기록
    m_currentTime = std::chrono::high_resolution_clock::now();

    // 이전 프레임과의 시간 간격 (std::chrono::duration으로 반환)
    auto delta = m_currentTime - m_prevTime;
    m_unscaledDeltaTime = std::chrono::duration<float>(delta).count();

    // timeScale을 곱한 deltaTime
    m_deltaTime = m_unscaledDeltaTime * m_timeScale;

    // 시작 이후 누적 시간
    std::chrono::duration<float> unscaledTotalDuration = m_currentTime - m_initTime;
    m_unscaledTotalTime = unscaledTotalDuration.count(); // 초 단위 float으로 변환

    m_totalTime = m_unscaledTotalTime * m_timeScale;

    // 이전 시간 갱신
    m_prevTime = m_currentTime;
}

void GOTOEngine::TimeManager::FixedUpdate()
{
	m_fixedTime += m_fixedDeltaTime;
}

