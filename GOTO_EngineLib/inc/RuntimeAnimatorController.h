#pragma once
#include "Object.h"
#include <unordered_map>
#include "AnimatorController.h"

namespace GOTOEngine
{
	class RuntimeAnimatorController : public Object
	{
	private:
		AnimatorController* m_controller;

		//parameter
		std::vector<bool> m_triggers;
		std::vector<bool> m_bools;
		std::vector<int> m_ints;
		std::vector<float> m_floats;

		std::unordered_map<std::string, size_t> m_triggerStrHash;
		std::unordered_map<std::string, size_t> m_boolStrHash;
		std::unordered_map<std::string, size_t> m_intStrHash;
		std::unordered_map<std::string, size_t> m_floatStrHash;

		std::unordered_map<std::wstring, size_t> m_triggerWStrHash;
		std::unordered_map<std::wstring, size_t> m_boolWStrHash;
		std::unordered_map<std::wstring, size_t> m_intWStrHash;
		std::unordered_map<std::wstring, size_t> m_floatWStrHash;

		//state
		AnimatorState* currentState;
		size_t m_currentKeyframeIdx;
		float m_time;

		bool CheckCondition(const AnimatorCondition& condition);

		void Dispose() override;
	public:
		RuntimeAnimatorController(AnimatorController* controller);
		void Update(float deltaTime);
		void RegisterTrigger(std::string name, bool defaultvalue);
		void RegisterBool(std::string name, bool defaultvalue);
		void RegisterInt(std::string name, int defaultvalue);
		void RegisterFloat(std::string name, float defaultvalue);

		//string
		bool GetTrigger(std::string name) { if (m_triggerStrHash.find(name) != m_triggerStrHash.end()) return m_triggers[m_triggerStrHash[name]]; return false; }
		bool GetBool(std::string name) { if (m_boolStrHash.find(name) != m_boolStrHash.end()) return m_bools[m_boolStrHash[name]]; return false; }
		int GetInt(std::string name) { if (m_intStrHash.find(name) != m_intStrHash.end()) return m_ints[m_intStrHash[name]]; return 0; }
		float GetFloat(std::string name) { if (m_floatStrHash.find(name) != m_floatStrHash.end()) return m_floats[m_floatStrHash[name]]; return 0.0f; }

		void SetTrigger(std::string name) { if (m_triggerStrHash.find(name) != m_triggerStrHash.end()) m_triggers[m_triggerStrHash[name]] = true; }
		void ResetTrigger(std::string name) { if (m_triggerStrHash.find(name) != m_triggerStrHash.end()) m_triggers[m_triggerStrHash[name]] = false; }
		void SetBool(std::string name, bool value) { if (m_boolStrHash.find(name) != m_boolStrHash.end()) m_bools[m_boolStrHash[name]] = value; }
		void SetInt(std::string name, int value) { if (m_intStrHash.find(name) != m_intStrHash.end()) m_ints[m_intStrHash[name]] = value; }
		void SetFloat(std::string name, float value) { if (m_floatStrHash.find(name) != m_floatStrHash.end()) m_floats[m_floatStrHash[name]] = value; }

		//wstring
		bool GetTrigger(std::wstring name) { if (m_triggerWStrHash.find(name) != m_triggerWStrHash.end()) return m_triggers[m_triggerWStrHash[name]]; return false; }
		bool GetBool(std::wstring name) { if (m_boolWStrHash.find(name) != m_boolWStrHash.end()) return m_bools[m_boolWStrHash[name]]; return false; }
		int GetInt(std::wstring name) { if (m_intWStrHash.find(name) != m_intWStrHash.end()) return m_ints[m_intWStrHash[name]]; return 0; }
	    float GetFloat(std::wstring name) { if (m_floatWStrHash.find(name) != m_floatWStrHash.end()) return m_floats[m_floatWStrHash[name]]; return 0.0f; }
		
		void SetTrigger(std::wstring name) { if (m_triggerWStrHash.find(name) != m_triggerWStrHash.end()) m_triggers[m_triggerWStrHash[name]] = true; }
		void ResetTrigger(std::wstring name) { if (m_triggerWStrHash.find(name) != m_triggerWStrHash.end()) m_triggers[m_triggerWStrHash[name]] = false; }
		void SetBool(std::wstring name, bool value) { if (m_boolWStrHash.find(name) != m_boolWStrHash.end()) m_bools[m_boolWStrHash[name]] = value; }
		void SetInt(std::wstring name, int value) { if (m_intWStrHash.find(name) != m_intWStrHash.end()) m_ints[m_intWStrHash[name]] = value; }
		void SetFloat(std::wstring name, float value) { if (m_floatWStrHash.find(name) != m_floatWStrHash.end()) m_floats[m_floatWStrHash[name]] = value; }
	};
}