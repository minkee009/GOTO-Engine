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
		in.at("name").get_to(out.name);
		in.at("type").get_to(out.type);
		in.at("defaultFloat").get_to(out.defaultFloat);
		in.at("defaultInt").get_to(out.defaultInt);
		in.at("defaultBool").get_to(out.defaultBool);
	}

	void from_json(const nlohmann::json& in, GOTOEngine::AnimatorCondition& out) {
		in.at("parameter").get_to(out.parameter);
		in.at("mode").get_to(out.mode);
		in.at("threshold").get_to(out.threshold);
	}

	void from_json(const nlohmann::json& in, GOTOEngine::AnimatorTransition& out) {
		in.at("fromState").get_to(out.fromState);
		in.at("toState").get_to(out.toState);
		if (in.contains("conditions")) {
			out.conditions = in.at("conditions").get<std::vector<GOTOEngine::AnimatorCondition>>();
		}
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
				m_states.insert({ WStringHelper::string_to_wstring(stateInfo["name"]), state });

				if (stateInfo.contains("transitions"))
					state->m_transitions = stateInfo["transitions"].get<std::vector<AnimatorTransition>>();
			}
		}
	}
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