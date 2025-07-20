#pragma once
#include "Resource.h"
#include "AnimationClip.h"
#include <unordered_map>

namespace GOTOEngine
{

	struct AnimatorParameter
	{

	};

	//functor
	struct AnimatorCondition
	{

	};

	struct AnimatorTransition
	{
		std::wstring fromState;
		std::wstring toState;
		std::vector<AnimatorCondition> conditions;
	};

	class AnimatorState : public Object
	{
	private:
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
		const float& GetDuration() const { m_clip->m_duration; }
		const std::vector<AnimatorTransition>& GetTransitions() { return m_transitions; }
	};

	class AnimatorController : public Resource
	{
	private:
		friend class ResourceManager;
		std::unordered_map<std::wstring, AnimatorState*> m_states;
		void LoadFromFilePath(const std::wstring& filePath) override;
		bool IsValidData() override { return m_states.size() > 0; }
	};
}