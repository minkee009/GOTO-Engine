#include "RuntimeAnimatorController.h"

bool GOTOEngine::RuntimeAnimatorController::CheckCondition(const AnimatorCondition& condition)
{
	std::string param = condition.parameter;
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

}

GOTOEngine::RuntimeAnimatorController::RuntimeAnimatorController(AnimatorController* controller)
{
	//지역변수 초기화
	m_controller = controller;

	if (m_controller)
	{
		size_t triggerHashCount = 0;
		size_t boolHashCount = 0;
		size_t intHashCount = 0;
		size_t floatHashCount = 0;

		for (auto param : m_controller->m_parameters)
		{
			if (param.type == "Trigger") {
				RegisterTrigger(param.name, param.defaultBool);
				m_triggerStrHash[param.name] = triggerHashCount;
				m_triggerWStrHash[WStringHelper::string_to_wstring(param.name)] = triggerHashCount;

				triggerHashCount++;
			}
			else if (param.type == "Bool") {
				RegisterBool(param.name, param.defaultBool);
				m_boolStrHash[param.name] = boolHashCount;
				m_boolWStrHash[WStringHelper::string_to_wstring(param.name)] = boolHashCount;

				boolHashCount++;
			}
			else if (param.type == "Int") {
				RegisterInt(param.name, param.defaultInt);
				m_intStrHash[param.name] = intHashCount;
				m_intWStrHash[WStringHelper::string_to_wstring(param.name)] = intHashCount;

				intHashCount++;
			}
			else if (param.type == "Float") {
				RegisterInt(param.name, param.defaultFloat);
				m_floatStrHash[param.name] = floatHashCount;
				m_floatWStrHash[WStringHelper::string_to_wstring(param.name)] = floatHashCount;

				floatHashCount++;
			}
		}


	}
}

void GOTOEngine::RuntimeAnimatorController::Update(float deltaTime)
{
}

void GOTOEngine::RuntimeAnimatorController::RegisterTrigger(std::string name, bool defaultvalue)
{
}

void GOTOEngine::RuntimeAnimatorController::RegisterBool(std::string name, bool defaultvalue)
{
}

void GOTOEngine::RuntimeAnimatorController::RegisterInt(std::string name, int defaultvalue)
{
}

void GOTOEngine::RuntimeAnimatorController::RegisterFloat(std::string name, float defaultvalue)
{
}
