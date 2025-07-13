#include "AnimationController.h"

void AnimationState::ResetState()
{
	m_currentImage = m_clip->GetKeyFrameImages()[0];
	m_frameIdx = 0;
	m_frameTime = 0.0f;
	m_isCycleComplete = false;
}

void AnimationState::UpdateFrame(float deltaTime)
{
	m_frameTime += deltaTime;
	if (m_frameTime > m_clip->GetFrameRate())
	{
		float frameRate = m_clip->GetFrameRate();
		int frameCount = static_cast<int>(m_frameTime / frameRate);
		m_frameTime -= frameCount * frameRate;

		for (int i = 0; i < frameCount; i++)
		{
			m_frameIdx++;
			if (m_clip->IsLoop())
			{
				m_frameIdx %= m_clip->GetLength();
				if (m_frameIdx == 0)
					m_isCycleComplete = true;
			}
			else
			{
				if (m_frameIdx == m_clip->GetLength())
					m_isCycleComplete = true;
				m_frameIdx = m_frameIdx < m_clip->GetLength() ? m_frameIdx : m_clip->GetLength() - 1;
			}
		}
	}
	m_currentImage = m_clip->GetKeyFrameImages()[m_frameIdx];
}

AnimationController::~AnimationController()
{
	for (auto& state : m_animationStates)
	{
		if (state.second)
			delete state.second;
	}
	m_animationStates.clear();
}

void AnimationController::AddAnimationState(const std::wstring& name, const AnimationClip& clip)
{
	m_animationStates[name] = new AnimationState(name, clip);
}

void AnimationController::Play(const std::wstring& name)
{
	auto it = m_animationStates.find(name);
	m_currentState = it->second;
	m_currentState->ResetState();
}

void AnimationController::Update(float deltaTime)
{
	if (!m_currentState)
		return;

	m_currentState->UpdateFrame(deltaTime);
}

bool AnimationController::IsPlaying(const std::wstring& name)
{
	auto it = m_animationStates.find(name);

	return m_currentState == it->second;
}
