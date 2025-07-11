#include "Transform.h"
#include "WStringHelper.h"

void GOTOEngine::Transform::AddChild(Transform* child)
{
	if (child == nullptr || child == this) return; // 자기 자신이나 nullptr는 추가하지 않음
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
	if (child == nullptr) return; // nullptr는 제거하지 않음
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
	: m_localPosition(0.0f, 0.0f)
	, m_localRotation(0.0f)
	, m_localScale(1.0f, 1.0f)
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

	//자식의 배열자 위치를 찾음
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

const GOTOEngine::Vector2 GOTOEngine::Transform::GetPosition() const
{
	if (m_parent)
	{
		// 부모 월드 행렬을 활용
		Matrix3x3 parentWorld = m_parent->GetWorldMatrix();
		return parentWorld.MultiplyPoint(m_localPosition);
	}
	else
	{
		return m_localPosition;
	}
}

float GOTOEngine::Transform::GetRotation() const
{
	if (m_parent)
	{
		// 부모가 있다면 부모의 회전을 적용
		return m_parent->GetRotation() + m_localRotation;
	}
	else
		return m_localRotation;
}

const GOTOEngine::Vector2 GOTOEngine::Transform::GetScale() const
{
	if (m_parent)
	{
		// 부모 스케일과 곱하여 월드 스케일 계산
		Vector2 parentScale = m_parent->GetScale();
		return Vector2(m_localScale.x * parentScale.x, m_localScale.y * parentScale.y);
	}
	else
	{
		return m_localScale;
	}
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
	Matrix3x3 worldMatrixBefore = GetWorldMatrix();

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
			// 부모 기준 상 로컬 행렬
			Matrix3x3 parentWorldInv = m_parent->GetWorldMatrix().Inverse();
			Matrix3x3 newLocal = parentWorldInv * worldMatrixBefore;

			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetScale();
		}
		else
		{
			// 부모가 없으면 로컬 = 월드
			m_localPosition = worldMatrixBefore.GetPosition();
			m_localRotation = worldMatrixBefore.GetRotation();
			m_localScale = worldMatrixBefore.GetScale();
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

const GOTOEngine::Transform* GOTOEngine::Transform::GetRoot() const
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

void GOTOEngine::Transform::LookAt(const Vector2& target)
{
	Vector2 direction = target - GetPosition();
	float angle = std::atan2(direction.y, direction.x) * Mathf::Rad2Deg;

	if (m_parent)
	{
		// 부모가 있으면 부모의 회전을 빼서 로컬 회전 계산
		m_localRotation = angle - m_parent->GetRotation();
	}
	else
	{
		m_localRotation = angle;
	}

	m_localRotation = Mathf::NormalizeAngle(m_localRotation);
	MarkDirty();
}

void GOTOEngine::Transform::Rotate(float angle)
{
	m_localRotation += angle;

	m_localRotation = Mathf::NormalizeAngle(m_localRotation);
	MarkDirty();
}

void GOTOEngine::Transform::Translate(const Vector2& translation, bool worldSpace)
{
	if (worldSpace)
	{
		// 월드 공간에서 이동
		Vector2 worldPos = GetPosition() + translation;
		SetPosition(worldPos);
	}
	else
	{
		// 로컬 공간에서 이동 (회전 적용)
		float radians = m_localRotation * Mathf::Deg2Rad;
		float cos_r = std::cos(radians);
		float sin_r = std::sin(radians);

		Vector2 rotatedTranslation(
			translation.x * cos_r - translation.y * sin_r,
			translation.x * sin_r + translation.y * cos_r
		);

		m_localPosition += rotatedTranslation;
	}
	MarkDirty();
}

GOTOEngine::Vector2 GOTOEngine::Transform::TransformDirection(const Vector2& direction) const
{
	float worldRotation = GetRotation() * Mathf::Deg2Rad;
	float cos_r = std::cos(worldRotation);
	float sin_r = std::sin(worldRotation);

	return Vector2(
		direction.x * cos_r - direction.y * sin_r,
		direction.x * sin_r + direction.y * cos_r
	);
}

GOTOEngine::Vector2 GOTOEngine::Transform::InverseTransformDirection(const Vector2& direction) const
{
	// TransformDirection의 역변환입니다.
	float worldRotation = -GetRotation() * Mathf::Deg2Rad; // 역회전
	float cos_r = std::cos(worldRotation);
	float sin_r = std::sin(worldRotation);

	return Vector2(
		direction.x * cos_r - direction.y * sin_r,
		direction.x * sin_r + direction.y * cos_r
	);
}

GOTOEngine::Vector2 GOTOEngine::Transform::TransformPoint(const Vector2& point) const
{
	Matrix3x3 worldMatrix = GetWorldMatrix();
	return worldMatrix.MultiplyPoint(point);
}

GOTOEngine::Vector2 GOTOEngine::Transform::InverseTransformPoint(const Vector2& point) const
{
	Matrix3x3 worldMatrix = GetWorldMatrix();
	Matrix3x3 inverseWorldMatrix = worldMatrix.Inverse();
	return inverseWorldMatrix.MultiplyPoint(point);
}

GOTOEngine::Matrix3x3 GOTOEngine::Transform::GetLocalMatrix() const
{
	if (m_isMatrixDirty)
	{
		m_cachedMatrix = Matrix3x3::TRS(
			m_localPosition,
			m_localRotation * Mathf::Deg2Rad, // degree를 radian으로 변환
			m_localScale
		);
		m_isMatrixDirty = false;
	}
	return m_cachedMatrix;
}

GOTOEngine::Matrix3x3 GOTOEngine::Transform::GetWorldMatrix() const
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

GOTOEngine::Vector2 GOTOEngine::Transform::GetRight() const
{
	float worldRotation = GetRotation() * Mathf::Deg2Rad;
	return Vector2(std::cos(worldRotation), std::sin(worldRotation));
}

GOTOEngine::Vector2 GOTOEngine::Transform::GetUp() const
{
	float worldRotation = GetRotation() * Mathf::Deg2Rad;
	return Vector2(-std::sin(worldRotation), std::cos(worldRotation));
}