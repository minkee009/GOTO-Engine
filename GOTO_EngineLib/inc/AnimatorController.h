#pragma once
#include "Resource.h"
#include "AnimationClip.h"
#include <unordered_map>

namespace GOTOEngine
{
	enum class AnimatorParameterType { Int, Float, Bool, Trigger };

	struct AnimatorParameter
	{
		std::string name;
		AnimatorParameterType type;
		float defaultFloat = 0.0f;
		int defaultInt = 0; 
		bool defaultBool = false;
	};

	//functor
	struct AnimatorCondition
	{
		std::string parameter; // ���� �Ķ���� �̸�
		std::string mode; // ���� ��� (��: Greater, IfNot ��)
		AnimatorParameterType type; // �Ķ���� Ÿ�� (��: Int, Float, Bool, Trigger)
		float threshold = 0.0f; // ���� �Ӱ谪
	};

	struct AnimatorTransition
	{
		std::string fromState;
		std::string toState;
		std::vector<AnimatorCondition> conditions;
	};

	class AnimatorState : public Object
	{
	private:
		friend class AnimatorController;
		AnimationClip* m_clip;
		std::vector<AnimatorTransition> m_transitions;

		void Dispose() override
		{
			if (IsValidObject(m_clip)
				&& !m_clip->IsDestroyed())
			{
				DestroyImmediate(m_clip);
				m_clip = nullptr;
			}
		}
	public:
		AnimatorState(AnimationClip* clip)
			: m_clip(clip)
		{

		}
		const float& GetDuration() const { m_clip->m_duration; }
		const std::vector<AnimatorTransition>& GetTransitions() { return m_transitions; }
	};

	class AnimatorController : public Resource
	{
	private:
		friend class ResourceManager;
		std::string m_defaultState;
		std::unordered_map<std::string, AnimatorState*> m_states;
		std::vector<AnimatorParameter> m_parameters;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidData() override { return m_states.size() > 0; }
	public:
		AnimatorState* GetState(std::string name);
		const std::vector<AnimatorParameter>& GetParameters() const { return m_parameters; }
	};
}