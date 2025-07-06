#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Component.h"
#include "Matrix4x4.h"
#include <cmath>

namespace GOTOEngine 
{
	class Transform : public Component
	{
	private:
		friend class GameObject;

		Vector3 m_localPosition;
		Quaternion m_localRotation;
		Vector3 m_localScale;

		Transform* m_parent;
		std::vector<Transform*> m_childs;		

		mutable bool m_isMatrixDirty = false;
		mutable Matrix4x4 m_cachedMatrix; // 캐시된 월드 행렬

		void AddChild(Transform* child);
		void RemoveChild(Transform* child);
		void MarkDirty();
	public:
		Transform();

		// 트랜스폼 프로퍼티
		void SetPosition(const Vector3& position) { MarkDirty(); m_localPosition = position; }
		const Vector3& GetLocalPosition() const { return m_localPosition; }
		const Vector3 GetPosition() const;

		void SetRotation(const Quaternion& rotation) { MarkDirty(); m_localRotation = rotation; }
		const Quaternion& GetLocalRotation() const { return m_localRotation; }
		Quaternion GetRotation() const;
		void SetEulerAngle(const Vector3& angles);
		const Vector3 GetLocalEulerAngle() const { return m_localRotation.ToEulerAngles(); }
		Vector3 GetEulerAngle() const;

		void SetLossyScale(const Vector3& scale) { MarkDirty(); m_localScale = scale; }
		const Vector3& GetLocalScale() const { return m_localScale; }
		const Vector3 GetLossyScale() const;

		// 씬 그래프
		void SetParent(Transform* parent);
		void SetParent(Transform* parent, bool worldPositionStays);
		Transform* GetParent() const { return m_parent; }
		const Transform* GetChild(size_t idx) const;
		size_t GetChildCount() const { return m_childs.size(); }
		Transform* Find(const std::wstring& name);
		const Transform* GetRoot() const;
		void DetachChildren();

		void SetSiblingIndex(size_t idx);
		size_t GetSiblingIndex() const;

		// 월드 공간 변환
		void LookAt(const Vector3& target, const Vector3& worldUp);
		void Rotate(const Vector3& eulerAngles, bool worldSpace = false);
		void Translate(const Vector3& translation, bool worldSpace);

		void RotateAround(const Vector3& point, const Vector3& axis, float angle);
		Vector3 TransformDirection(const Vector3& direction) const;
		Vector3 InverseTransformDirection(const Vector3& direction) const;

		// 행렬
		Matrix4x4 GetLocalMatrix() const;
		Matrix4x4 GetWorldMatrix() const;
	};
}