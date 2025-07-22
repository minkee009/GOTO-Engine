#pragma once
#include "Resource.h"
#include "AnimationClip.h"
#include <unordered_map>

namespace GOTOEngine
{
	enum class AnimatorParameterType { Int, Float, Bool, Trigger };

	struct AnimatorParameter
	{
		std::wstring name;
		std::string type;
		float defaultFloat = 0.0f;
		int defaultInt = 0; 
		bool defaultBool = false;
	};

	//functor
	struct AnimatorCondition
	{
		std::wstring parameter; // ���� �Ķ���� �̸�
		std::string mode; // ���� ��� (��: Greater, IfNot ��)
		AnimatorParameterType type; // �Ķ���� Ÿ�� (��: Int, Float, Bool, Trigger)
		float threshold = 0.0f; // ���� �Ӱ谪
	};

	struct AnimatorTransition
	{
		std::wstring fromState;
		std::wstring toState;
		bool hasExitTime = false; // ���� �ð��� �ִ��� ����
		float exitTime = -1.0f; // 1.0�̸� 100% �Ϸ� �� ����, -1.0�̸� ���� �ð� ����
		std::vector<AnimatorCondition> conditions;
	};

	//Load<T>(filePath)�� �ҷ��� �� ���� - ���� Ŭ����
	class AnimatorState : public Object
	{
	private:
		friend class AnimatorController;
		friend class RuntimeAnimatorController;
		AnimationClip* m_clip;
		std::vector<AnimatorTransition> m_transitions;
		std::wstring m_stateName;

		void Dispose() override
		{
			if (IsValidObject(m_clip)
				&& !m_clip->IsDestroyed())
			{
				m_clip->DecreaseRefCount();
				m_clip = nullptr;
			}
		}
	public:
		AnimatorState(AnimationClip* clip)
			: m_clip(clip)
		{
			if (m_clip)
				m_clip->IncreaseRefCount();
		}
		const float& GetDuration() const { return m_clip->m_duration; }
		const std::wstring& GetStateName() const { return m_stateName; }
		const std::vector<AnimatorTransition>& GetTransitions() const { return m_transitions; }
	};

	class AnimatorController : public Resource
	{
	private:
		friend class ResourceManager;
		friend class RuntimeAnimatorController;
		std::wstring m_defaultState;
		std::unordered_map<std::wstring, AnimatorState*> m_states;
		std::vector<AnimatorParameter> m_parameters;
		std::unordered_map<std::wstring, AnimatorParameterType> m_paramTypes;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidData() override { return m_states.size() > 0; }
		AnimatorParameterType stringToParamType(const std::string& type);
	public:
		void Dispose() override;
		AnimatorState* GetState(std::wstring name);
		const std::vector<AnimatorParameter>& GetParameters() const { return m_parameters; }
	};
}