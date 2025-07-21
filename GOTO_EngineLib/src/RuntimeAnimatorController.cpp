#include "RuntimeAnimatorController.h"
#include "Mathf.h"

bool GOTOEngine::RuntimeAnimatorController::CheckCondition(const AnimatorCondition& condition)
{
	std::wstring param = condition.parameter;
	std::string mode = condition.mode;
	float threshold = condition.threshold;

	switch (condition.type)
	{
	case AnimatorParameterType::Bool:
	{
		bool val = GetBool(param);
		if (mode == "If") return val;
		if (mode == "IfNot") return !val;
		break;
	}
	case AnimatorParameterType::Int:
	{
		int val = GetInt(param);
		if (mode == "Greater") return val > threshold;
		if (mode == "Less") return val < threshold;
		if (mode == "Equals") return val == static_cast<int>(threshold);
		if (mode == "NotEqual") return val != static_cast<int>(threshold);
		break;
	}
	case AnimatorParameterType::Float:
	{
		float val = GetFloat(param);
		if (mode == "Greater") return val > threshold;
		if (mode == "Less") return val < threshold;
		if (mode == "Equals") return val == threshold;
		if (mode == "NotEqual") return val != threshold;
		break;
	}
	case AnimatorParameterType::Trigger:
	{
		bool val = GetTrigger(param);
		if (mode == "If") return val;
		if (mode == "IfNot") return !val;
		break;
	}
	default:
		return false;
	}
	return false;
}
void GOTOEngine::RuntimeAnimatorController::Dispose()
{
	if(IsValidObject(m_controller) && !m_controller->IsDestroyed())
		m_controller->DecreaseRefCount();
}

GOTOEngine::RuntimeAnimatorController::RuntimeAnimatorController(AnimatorController* controller)
{
	//지역변수 초기화
	m_controller = controller;
	m_currentState = nullptr;

	if (m_controller)
	{
		m_controller->IncreaseRefCount();
		for (auto param : m_controller->m_parameters)
		{
			if (param.type == "Trigger") {
				RegisterTrigger(param.name, param.defaultBool);
			}
			else if (param.type == "Bool") {
				RegisterBool(param.name, param.defaultBool);
			}
			else if (param.type == "Int") {
				RegisterInt(param.name, param.defaultInt);
			}
			else if (param.type == "Float") {
				RegisterInt(param.name, param.defaultFloat);
			}
		}

		m_currentState = m_controller->GetState(m_controller->m_defaultState);
	}

	m_time = 0.0f;
}

void GOTOEngine::RuntimeAnimatorController::Update(float deltaTime)
{
	for (auto transition : m_currentState->GetTransitions())
	{
		bool allConditionsMet = true;
		for (auto& condition : transition.conditions)
		{
			if (!CheckCondition(condition)) {
				allConditionsMet = false;
				break;
			}
		}

		if (allConditionsMet)
		{
			m_currentState = m_controller->GetState(transition.toState);
			m_time = 0.0f; // 애니메이션 시간 리셋
			break;
		}
	}

	auto currentClip = m_currentState->m_clip;
	if(!currentClip->GetKeyframes().empty())
		m_time += deltaTime;
	auto curretnClipDuration = currentClip->GetDuration();

	if (currentClip->IsLoop()) {
		while (m_time > curretnClipDuration) {
			m_time -= curretnClipDuration;
		}
	}
	else {
		if (m_time > curretnClipDuration) {
			m_time = curretnClipDuration;
		}
	}
}

GOTOEngine::Sprite* GOTOEngine::RuntimeAnimatorController::GetCurrentSprite()
{
	auto currentKeyframes = m_currentState->m_clip->GetKeyframes();

	// 뒤에서부터 검색하여 현재 시간보다 작거나 같은 프레임을 찾음
	for (int i = static_cast<int>(currentKeyframes.size()) - 1; i >= 0; --i) {
		if (m_time >= currentKeyframes[i]->GetTime()) {
			return currentKeyframes[i]->GetSprite();
		}
	}
	//0번째 이미지 출력
	return currentKeyframes.front()->GetSprite();
}

void GOTOEngine::RuntimeAnimatorController::RegisterTrigger(std::wstring name, bool defaultvalue)
{
	m_triggers[name] = defaultvalue;
}

void GOTOEngine::RuntimeAnimatorController::RegisterBool(std::wstring name, bool defaultvalue)
{
	m_bools[name] = defaultvalue;
}

void GOTOEngine::RuntimeAnimatorController::RegisterInt(std::wstring name, int defaultvalue)
{
	m_ints[name] = defaultvalue;
}

void GOTOEngine::RuntimeAnimatorController::RegisterFloat(std::wstring name, float defaultvalue)
{
	m_floats[name] = defaultvalue;
}
