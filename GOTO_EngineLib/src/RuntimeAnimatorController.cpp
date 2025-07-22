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
	//�������� �ʱ�ȭ
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
				RegisterFloat(param.name, param.defaultFloat);
			}
		}

		m_currentState = m_controller->GetState(m_controller->m_defaultState);
	}

	m_time = 0.0f;
}

void GOTOEngine::RuntimeAnimatorController::Update(float deltaTime)
{
	auto currentClip = m_currentState->m_clip;

	if (currentClip && !currentClip->GetKeyframes().empty())
		m_time += deltaTime;

	float currentClipDuration = currentClip->GetDuration();

	if (currentClip->IsLoop()) {
		if (currentClipDuration > 0.0f) {
			m_time = std::fmod(m_time, currentClipDuration);
		}
	}
	else {
		if (m_time > currentClipDuration) {
			m_time = currentClipDuration;
		}
	}

	float normalizedTime = (currentClipDuration > 0.0f) ? (m_time / currentClipDuration) : 0.0f;

	for (const auto& transition : m_currentState->GetTransitions())
	{
		// 1. ExitTime ���� Ȯ��
		if (transition.hasExitTime)
		{
			if (normalizedTime < transition.exitTime)
				continue; // ���� ���� �������� ����
		}

		// 2. ���� ���� Ȯ��
		bool allConditionsMet = true;
		std::vector<std::wstring> savedTriggers;
		for (const auto& condition : transition.conditions)
		{
			if (!CheckCondition(condition)) {
				allConditionsMet = false;
				break;
			}

			if (condition.type == AnimatorParameterType::Trigger)
			{
				savedTriggers.emplace_back(condition.parameter);
			}
		}

		if (allConditionsMet)
		{
			for (auto& trigger : savedTriggers)
			{
				ResetTrigger(trigger);
			}

			m_currentState = m_controller->GetState(transition.toState);
			m_time = 0.0f; // �ִϸ��̼� �ð� �ʱ�ȭ
			break;
		}
	}
}

GOTOEngine::Sprite* GOTOEngine::RuntimeAnimatorController::GetCurrentSprite()
{
	auto currentKeyframes = m_currentState->m_clip->GetKeyframes();

	// �ڿ������� �˻��Ͽ� ���� �ð����� �۰ų� ���� �������� ã��
	for (int i = static_cast<int>(currentKeyframes.size()) - 1; i >= 0; --i) {
		if (m_time >= currentKeyframes[i]->GetTime()) {
			return currentKeyframes[i]->GetSprite();
		}
	}
	//0��° �̹��� ���
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
