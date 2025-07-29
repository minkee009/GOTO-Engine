#pragma once
#include "Vector2.h"
#include "Component.h"
#include "Matrix3x3.h"
#include "Mathf.h"

namespace GOTOEngine
{
	class Transform : public Component
	{
	protected:
		friend class GameObject;

		Vector2 m_localPosition;
		float m_localRotation; // degree 단위
		Vector2 m_localScale;

		Transform* m_parent;
		std::vector<Transform*> m_childs;

		mutable bool m_isMatrixDirty = false;
		mutable Matrix3x3 m_cachedMatrix; // 캐시된 월드 행렬

		void AddChild(Transform* child);
		void RemoveChild(Transform* child);
		void MarkDirty();
	public:
		Transform();

		// 트랜스폼 프로퍼티
		void SetPosition(const Vector2& position);
		const Vector2 GetPosition() const;

		void SetRotation(float rotation);
		float GetRotation() const;

		void SetLossyScale(const Vector2& scale);
		const Vector2 GetLossyScale() const;

		void SetLocalPosition(const Vector2& position) { MarkDirty(); m_localPosition = position; }
		const Vector2& GetLocalPosition() const { return m_localPosition; }

		void SetLocalRotation(float rotation) { MarkDirty(); m_localRotation = rotation; }
		float GetLocalRotation() const { return m_localRotation; }

		void SetLocalScale(const Vector2& scale) { MarkDirty(); m_localScale = scale; }
		const Vector2& GetLocalScale() const { return m_localScale; }

		// 자식 그래프
		void SetParent(Transform* parent);
		virtual void SetParent(Transform* parent, bool worldPositionStays);
		Transform* GetParent() const { return m_parent; }
		Transform* GetChild(size_t idx);
		size_t GetChildCount() const { return m_childs.size(); }
		Transform* Find(const std::wstring& name);
		Transform* GetRoot();
		void DetachChildren();

		virtual void SetSiblingIndex(size_t idx);
		size_t GetSiblingIndex() const;

		// 월드 공간 변환
		void LookAt(const Vector2& target);
		void Rotate(float angle);
		void Translate(const Vector2& translation, bool worldSpace = false);

		Vector2 TransformDirection(const Vector2& direction) const;
		Vector2 InverseTransformDirection(const Vector2& direction) const;
		Vector2 TransformPoint(const Vector2& point) const;
		Vector2 InverseTransformPoint(const Vector2& point) const;

		// 행렬
		Matrix3x3 GetLocalMatrix() const;
		Matrix3x3 GetWorldMatrix() const;

		// 2D 전용 유틸리티
		Vector2 GetRight() const; // 로컬 X축 방향
		Vector2 GetUp() const;    // 로컬 Y축 방향
		Vector2 GetForward() const { return GetUp(); } // 2D에서는 Up이 Forward
	};
}