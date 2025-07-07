#pragma once
#include "Behaviour.h"

namespace GOTOEngine 
{
	class ScriptBehaviour : public Behaviour
	{
	public:
		ScriptBehaviour() = default;
		virtual ~ScriptBehaviour() { OnDisable(); OnDestroy(); }

		virtual void Awake() {}
		virtual void Start() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void FixedUpdate() {}
		virtual void PreRender() {}
		virtual void PostRender() {}
		virtual void OnDestroy() {}
	};
}