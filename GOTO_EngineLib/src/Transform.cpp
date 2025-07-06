#include "Transform.h"
#include "WStringHelper.h"

void GOTOEngine::Transform::AddChild(Transform* child)
{
	if (child == nullptr || child == this) return; // 자기 자신이나 nullptr은 추가하지 않음
	// 이미 자식으로 등록되어 있는지 확인
	for (const auto& existingChild : m_childs)
	{
		if (existingChild == child) return; // 이미 자식으로 등록되어 있으면 추가하지 않음
	}
	m_childs.push_back(child);
	child->m_parent = this; // 부모 설정
}

void GOTOEngine::Transform::RemoveChild(Transform* child)
{
	if (child == nullptr) return; // nullptr은 제거하지 않음
	auto it = std::find(m_childs.begin(), m_childs.end(), child);
	if (it != m_childs.end())
	{
		m_childs.erase(it); // 자식 목록에서 제거
		child->m_parent = nullptr; // 부모 설정 해제
	}
}

void GOTOEngine::Transform::MarkDirty()
{
	m_isMatrixDirty = true; // 행렬이 더러워졌음을 표시
	for (Transform* child : m_childs)
	{
		child->MarkDirty(); // 자식들도 더러워졌다고 표시
	}
}

GOTOEngine::Transform::Transform() 
	: m_localPosition(0.0f, 0.0f, 0.0f)
	, m_localRotation(0.0f, 0.0f, 0.0f, 1.0f)
	, m_localScale(1.0f, 1.0f, 1.0f)
	, m_parent(nullptr)
	, m_isMatrixDirty(true) // 처음에는 행렬이 더러워져 있으므로
{
	m_cachedMatrix.SetIdentity();
}

void GOTOEngine::Transform::SetSiblingIndex(size_t idx)
{
	if (!m_parent)
		return;

	if (idx >= m_parent->m_childs.size())
	{
		// 인덱스가 범위를 벗어나면 아무 작업도 하지 않음
		return;
	}

	if (m_parent->m_childs[idx] == this)
	{
		// 이미 해당 인덱스에 있다면 아무 작업도 하지 않음
		return;
	}

	//자신의 반복자 위치를 찾음
	auto& children = m_parent->m_childs;
	auto current_it = std::find(children.begin(), children.end(), this);

	// 현재 인덱스 계산
	size_t current_idx = std::distance(children.begin(), current_it);

	//std::rotate를 사용하여 요소 이동
	if (current_idx < idx)
	{
		// 현재 인덱스가 목표 인덱스보다 작으면 오른쪽으로 이동
		std::rotate(children.begin() + current_idx,
			children.begin() + current_idx + 1,
			children.begin() + idx + 1);
	}
	else
	{
		// 현재 인덱스가 목표 인덱스보다 크면 왼쪽으로 이동
		std::rotate(children.begin() + idx,
			children.begin() + current_idx,
			children.begin() + current_idx + 1);
	}
}

size_t GOTOEngine::Transform::GetSiblingIndex() const
{
	if (m_parent)
	{
		auto it = std::find(m_parent->m_childs.begin(), m_parent->m_childs.end(), this);
		if (it != m_parent->m_childs.end())
		{
			return std::distance(m_parent->m_childs.begin(), it);
		}
	}
	return 0; // 부모가 없거나 찾을 수 없는 경우 0 반환
}

const GOTOEngine::Vector3 GOTOEngine::Transform::GetPosition() const
{
	if (m_parent)
	{
		// 부모 월드 행렬을 활용
		Matrix4x4 parentWorld = m_parent->GetWorldMatrix();
		return parentWorld.MultiplyPoint(m_localPosition);
	}
	else
	{
		return m_localPosition;
	}
}

GOTOEngine::Quaternion GOTOEngine::Transform::GetRotation() const
{
	if (m_parent)
	{
		// 부모가 있다면 부모의 회전을 적용
		return m_parent->GetRotation() * m_localRotation;
	}
	else
		return m_localRotation;
}

void GOTOEngine::Transform::SetEulerAngle(const Vector3& angles)
{
	MarkDirty();
	m_localRotation = Quaternion::Euler(angles);
}

GOTOEngine::Vector3 GOTOEngine::Transform::GetEulerAngle() const
{
	if (m_parent)
	{
		// 부모가 있다면 부모의 회전을 적용
		return GetRotation().ToEulerAngles();
	}
	else
		return m_localRotation.ToEulerAngles();
}

const GOTOEngine::Vector3 GOTOEngine::Transform::GetLossyScale() const
{
	Matrix4x4 world = GetWorldMatrix();

	Vector3 xAxis(
		world.m[0][0],
		world.m[1][0],
		world.m[2][0]);

	Vector3 yAxis(
		world.m[0][1],
		world.m[1][1],
		world.m[2][1]);

	Vector3 zAxis(
		world.m[0][2],
		world.m[1][2],
		world.m[2][2]);

	return Vector3(
		xAxis.Magnitude(),
		yAxis.Magnitude(),
		zAxis.Magnitude()
	);
}

void GOTOEngine::Transform::SetParent(Transform* parent)
{
	SetParent(parent, true); // 월드 포지션을 유지하면서 부모 설정
}

void GOTOEngine::Transform::SetParent(Transform* parent, bool worldPositionStays)
{
	// 이미 같은 부모면 return
	if (m_parent == parent) return;

	// 현재 월드 행렬 백업
	Matrix4x4 worldMatrixBefore = GetWorldMatrix();

	// 기존 부모에서 제거
	if (m_parent)
		m_parent->RemoveChild(this);

	// 부모 교체
	m_parent = parent;

	if (m_parent)
		m_parent->AddChild(this);

	if (worldPositionStays)
	{
		if (m_parent)
		{
			// 부모 기준 새 로컬 행렬
			Matrix4x4 parentWorldInv = m_parent->GetWorldMatrix().Inverse();
			Matrix4x4 newLocal = parentWorldInv * worldMatrixBefore;

			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetLossyScale();
		}
		else
		{
			// 부모가 없으면 로컬 = 월드
			Matrix4x4 newLocal = worldMatrixBefore;
			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetLossyScale();
		}
	}

	GetGameObject()->UpdateActiveInHierarchy(); // 부모가 바뀌었으므로 Hierarchy 활성화 상태 업데이트

	MarkDirty();
}

const GOTOEngine::Transform* GOTOEngine::Transform::GetChild(size_t idx) const
{
	if (idx >= m_childs.size())
		return nullptr;

	return m_childs[idx];
}


GOTOEngine::Transform* GOTOEngine::Transform::Find(const std::wstring& path)
{
	if (path.empty())
		return nullptr;

	// 1. 경로를 '/'로 분할
	std::vector<std::wstring> tokens = WStringHelper::Split(path, L'/');
	if (tokens.empty())
		return nullptr;

	// 2. 현재 Transform에서 시작
	Transform* current = this;

	// 3. 토큰 순회
	for (const auto& token : tokens)
	{
		bool found = false;

		for (Transform* child : current->m_childs)
		{
			if (child && child->name == token)
			{
				// 일치하는 자식 발견
				current = child;
				found = true;
				break;
			}
		}

		if (!found)
		{
			// 현재 레벨에서 이름을 못 찾으면 경로 실패
			return nullptr;
		}
	}

	return current;

}

const  GOTOEngine::Transform* GOTOEngine::Transform::GetRoot() const
{
	const Transform* current = this;

	while (current->m_parent != nullptr)
	{
		current = current->m_parent;
	}

	return current;
}

void GOTOEngine::Transform::DetachChildren()
{
	std::vector<Transform*> childrenCopy = m_childs;

	for (auto& child : childrenCopy)
	{
		child->SetParent(nullptr);
	}

	m_childs.clear();
}

void GOTOEngine::Transform::LookAt(const Vector3& target, const Vector3& worldUp)
{
	Vector3 forward = (target - GetPosition()).Normalized();
	Vector3 right = Vector3::Cross(worldUp, forward).Normalized();
	Vector3 up = Vector3::Cross(forward, right);

	// 회전 행렬을 쿼터니언으로 변환
	Matrix4x4 rotMat;
	rotMat.SetIdentity();

	// 열 우선 저장
	rotMat.m[0][0] = right.x;
	rotMat.m[1][0] = right.y;
	rotMat.m[2][0] = right.z;

	rotMat.m[0][1] = up.x;
	rotMat.m[1][1] = up.y;
	rotMat.m[2][1] = up.z;

	rotMat.m[0][2] = forward.x;
	rotMat.m[1][2] = forward.y;
	rotMat.m[2][2] = forward.z;

	m_localRotation = Quaternion::FromRotationMatrix(rotMat);
	MarkDirty();
}
void GOTOEngine::Transform::Rotate(const Vector3& eulerAngles, bool worldSpace)
{
	Quaternion delta = Quaternion::Euler(eulerAngles);
	m_localRotation = m_localRotation * delta;

	MarkDirty();
}
void GOTOEngine::Transform::Translate(const Vector3& translation, bool worldSpace)
{
	if (worldSpace)
	{
		m_localPosition += translation;
	}
	else
	{
		m_localPosition += m_localRotation * translation;
	}
	MarkDirty();
}

void GOTOEngine::Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
{
	// 현재 월드 위치
	Vector3 currentWorldPosition = GetPosition();

	// 회전 중심점으로부터 현재 위치까지의 벡터
	Vector3 offset = currentWorldPosition - point;

	// 회전 쿼터니언 생성
	Quaternion rotation = Quaternion::AngleAxis(angle, axis.Normalized());

	// offset 벡터를 회전
	Vector3 rotatedOffset = rotation * offset;

	// 새로운 월드 위치 계산
	Vector3 newWorldPosition = point + rotatedOffset;

	// 현재 Transform이 부모를 가지고 있다면, 새 월드 위치를 로컬 공간으로 변환해야 합니다.
	if (m_parent)
	{
		// 부모의 역행렬을 사용하여 월드 위치를 로컬 위치로 변환
		Matrix4x4 parentWorldMatrix = m_parent->GetWorldMatrix();
		Matrix4x4 parentInverseWorldMatrix = parentWorldMatrix.Inverse(); // 부모 월드 행렬의 역행렬

		m_localPosition = parentInverseWorldMatrix.MultiplyPoint(newWorldPosition);
	}
	else
	{
		m_localPosition = newWorldPosition;
	}

	// 현재 월드 회전 (normalized 된 상태)
	Quaternion currentWorldRotation = GetRotation().Normalized();

	// 적용할 회전 쿼터니언
	Quaternion addRotation = Quaternion::AngleAxis(angle, axis.Normalized());

	// 새로운 월드 회전
	Quaternion newWorldRotation = addRotation * currentWorldRotation; // 회전 순서에 유의

	if (m_parent)
	{
		// 부모의 월드 회전의 역변환을 사용하여 새로운 로컬 회전 계산
		Quaternion parentWorldRotationInverse = m_parent->GetRotation().Inverse();
		m_localRotation = parentWorldRotationInverse * newWorldRotation;
	}
	else
	{
		m_localRotation = newWorldRotation;
	}

	MarkDirty(); // 위치와 회전이 변경되었으므로 행렬을 더럽다고 표시
}

GOTOEngine::Vector3 GOTOEngine::Transform::TransformDirection(const Vector3& direction) const
{
	Quaternion worldRotation = GetRotation(); // 월드 회전 쿼터니언

	return worldRotation * direction;
}

GOTOEngine::Vector3 GOTOEngine::Transform::InverseTransformDirection(const Vector3& direction) const
{
	// TransformDirection의 역변환입니다.
	// 월드 회전의 켤레 복소수(역변환)를 사용합니다.
	Quaternion worldRotationInverse = GetRotation().Inverse();

	return worldRotationInverse * direction;
}

GOTOEngine::Matrix4x4 GOTOEngine::Transform::GetLocalMatrix() const
{
	if (m_isMatrixDirty)
	{
		m_cachedMatrix.SetIdentity();
		m_cachedMatrix = Matrix4x4::TRS(
			m_localPosition,
			m_localRotation,
			m_localScale
		);
		m_isMatrixDirty = false;
	}
	return m_cachedMatrix;
}

GOTOEngine::Matrix4x4 GOTOEngine::Transform::GetWorldMatrix() const
{
	if (m_parent)
	{
		return m_parent->GetWorldMatrix() * GetLocalMatrix();
	}
	else
	{
		return GetLocalMatrix();
	}
}
