#include "AnimatorController.h"
#include "WStringHelper.h"
#include <filesystem>
#include <fstream>
#include <json.hpp>
#include <iostream>


using json = nlohmann::json;
#ifdef _DEBUG
using std::wcout;
using std::cout;
using std::endl;
#endif

namespace GOTOEngine
{
	void from_json(const nlohmann::json& in, GOTOEngine::AnimatorParameter& out) {
		out.name = WStringHelper::string_to_wstring(in.at("name").get<std::string>());
		in.at("type").get_to(out.type);
		in.at("defaultFloat").get_to(out.defaultFloat);
		in.at("defaultInt").get_to(out.defaultInt);
		in.at("defaultBool").get_to(out.defaultBool);
	}

	void from_json(const nlohmann::json& in, GOTOEngine::AnimatorCondition& out) {
		out.parameter = WStringHelper::string_to_wstring(in.at("parameter").get<std::string>());
		in.at("mode").get_to(out.mode);
		in.at("threshold").get_to(out.threshold);
	}

	void from_json(const nlohmann::json& in, GOTOEngine::AnimatorTransition& out) {
		out.fromState = WStringHelper::string_to_wstring(in.at("fromState").get<std::string>());
		out.toState = WStringHelper::string_to_wstring(in.at("toState").get<std::string>());
		if (in.contains("conditions")) {
			out.conditions = in.at("conditions").get<std::vector<GOTOEngine::AnimatorCondition>>();
		}
		in.at("exitTime").get_to(out.exitTime);
		in.at("hasExitTime").get_to(out.hasExitTime);
	}
}




void GOTOEngine::AnimatorController::LoadFromFilePath(const std::wstring& filePath)
{
	std::ifstream inFile(filePath);
	if (inFile.is_open())
	{
		json jFromFile;
		inFile >> jFromFile;
		inFile.close();

		//��Ʈ�ѷ� ���� üũ
		if (!jFromFile.contains("commonClipPath"))
		{
#ifdef _DEBUG
			wcout << L"���ҽ� �ε� ���� : Common ClipPath�� ���� ���� �ʽ��ϴ�." << endl;
#endif
			return;
		}
		auto commonClipPath = WStringHelper::string_to_wstring(jFromFile["commonClipPath"]);

		//�Ķ���� ����
		m_parameters = jFromFile["parameters"].get<std::vector<AnimatorParameter>>();
		for (auto param : m_parameters)
		{
			m_paramTypes[param.name] = stringToParamType(param.type);
		}

		//�⺻ ���� ����
		m_defaultState = WStringHelper::string_to_wstring(jFromFile["defaultState"]);

		//������Ʈ ���� �� ����
		for (auto stateInfo : jFromFile["states"])
		{
			auto currentClipPath = commonClipPath + WStringHelper::string_to_wstring(stateInfo["motionName"]) + L"_anim.json";
			if (auto clip = Load<AnimationClip>(currentClipPath))
			{
				//Ŭ���� ������ ���� �����
				auto state = new AnimatorState(clip);
				state->m_stateName = WStringHelper::string_to_wstring(stateInfo["name"]);
				m_states.insert({ state->m_stateName, state });

				if (stateInfo.contains("transitions"))
					state->m_transitions = stateInfo["transitions"].get<std::vector<AnimatorTransition>>();

				for (auto& trans : state->m_transitions)
				{
					for (auto& condi : trans.conditions)
					{
						condi.type = m_paramTypes[condi.parameter];
					}
				}
			}
		}
	}
}

GOTOEngine::AnimatorParameterType GOTOEngine::AnimatorController::stringToParamType(const std::string& type)
{
	if (type == "Int") return AnimatorParameterType::Int;
	else if (type == "Float") return AnimatorParameterType::Float;
	else if (type == "Bool") return AnimatorParameterType::Bool;
	else if (type == "Trigger") return AnimatorParameterType::Trigger;
	return AnimatorParameterType::Int; // �⺻��
}

void GOTOEngine::AnimatorController::Dispose()
{
	for (auto it : m_states)
	{
		DestroyImmediate(it.second);
	}
	m_states.clear();
}

GOTOEngine::AnimatorState* GOTOEngine::AnimatorController::GetState(std::wstring name)
{
	auto it = m_states.find(name);
	if (it != m_states.end())
		return m_states[name];
	else
		return nullptr;
}