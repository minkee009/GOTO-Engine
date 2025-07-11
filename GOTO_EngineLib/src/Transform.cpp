#include "Transform.h"
#include "WStringHelper.h"

void GOTOEngine::Transform::AddChild(Transform* child)
{
	if (child == nullptr || child == this) return; // �ڱ� �ڽ��̳� nullptr�� �߰����� ����
	// �̹� �ڽ����� ��ϵǾ� �ִ��� Ȯ��
	for (const auto& existingChild : m_childs)
	{
		if (existingChild == child) return; // �̹� �ڽ����� ��ϵǾ� ������ �߰����� ����
	}
	m_childs.push_back(child);
	child->m_parent = this; // �θ� ����
}

void GOTOEngine::Transform::RemoveChild(Transform* child)
{
	if (child == nullptr) return; // nullptr�� �������� ����
	auto it = std::find(m_childs.begin(), m_childs.end(), child);
	if (it != m_childs.end())
	{
		m_childs.erase(it); // �ڽ� ��Ͽ��� ����
		child->m_parent = nullptr; // �θ� ���� ����
	}
}

void GOTOEngine::Transform::MarkDirty()
{
	m_isMatrixDirty = true; // ����� ������������ ǥ��
	for (Transform* child : m_childs)
	{
		child->MarkDirty(); // �ڽĵ鵵 ���������ٰ� ǥ��
	}
}

GOTOEngine::Transform::Transform()
	: m_localPosition(0.0f, 0.0f)
	, m_localRotation(0.0f)
	, m_localScale(1.0f, 1.0f)
	, m_parent(nullptr)
	, m_isMatrixDirty(true) // ó������ ����� �������� �����Ƿ�
{
	m_cachedMatrix.SetIdentity();
}

void GOTOEngine::Transform::SetSiblingIndex(size_t idx)
{
	if (!m_parent)
		return;

	if (idx >= m_parent->m_childs.size())
	{
		// �ε����� ������ ����� �ƹ� �۾��� ���� ����
		return;
	}

	if (m_parent->m_childs[idx] == this)
	{
		// �̹� �ش� �ε����� �ִٸ� �ƹ� �۾��� ���� ����
		return;
	}

	//�ڽ��� �迭�� ��ġ�� ã��
	auto& children = m_parent->m_childs;
	auto current_it = std::find(children.begin(), children.end(), this);

	// ���� �ε��� ���
	size_t current_idx = std::distance(children.begin(), current_it);

	//std::rotate�� ����Ͽ� ��� �̵�
	if (current_idx < idx)
	{
		// ���� �ε����� ��ǥ �ε������� ������ ���������� �̵�
		std::rotate(children.begin() + current_idx,
			children.begin() + current_idx + 1,
			children.begin() + idx + 1);
	}
	else
	{
		// ���� �ε����� ��ǥ �ε������� ũ�� �������� �̵�
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
	return 0; // �θ� ���ų� ã�� �� ���� ��� 0 ��ȯ
}

const GOTOEngine::Vector2 GOTOEngine::Transform::GetPosition() const
{
	if (m_parent)
	{
		// �θ� ���� ����� Ȱ��
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
		// �θ� �ִٸ� �θ��� ȸ���� ����
		return m_parent->GetRotation() + m_localRotation;
	}
	else
		return m_localRotation;
}

const GOTOEngine::Vector2 GOTOEngine::Transform::GetScale() const
{
	if (m_parent)
	{
		// �θ� �����ϰ� ���Ͽ� ���� ������ ���
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
	SetParent(parent, true); // ���� �������� �����ϸ鼭 �θ� ����
}

void GOTOEngine::Transform::SetParent(Transform* parent, bool worldPositionStays)
{
	// �̹� ���� �θ�� return
	if (m_parent == parent) return;

	// ���� ���� ��� ���
	Matrix3x3 worldMatrixBefore = GetWorldMatrix();

	// ���� �θ𿡼� ����
	if (m_parent)
		m_parent->RemoveChild(this);

	// �θ� ��ü
	m_parent = parent;

	if (m_parent)
		m_parent->AddChild(this);

	if (worldPositionStays)
	{
		if (m_parent)
		{
			// �θ� ���� �� ���� ���
			Matrix3x3 parentWorldInv = m_parent->GetWorldMatrix().Inverse();
			Matrix3x3 newLocal = parentWorldInv * worldMatrixBefore;

			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetScale();
		}
		else
		{
			// �θ� ������ ���� = ����
			m_localPosition = worldMatrixBefore.GetPosition();
			m_localRotation = worldMatrixBefore.GetRotation();
			m_localScale = worldMatrixBefore.GetScale();
		}
	}

	GetGameObject()->UpdateActiveInHierarchy(); // �θ� �ٲ�����Ƿ� Hierarchy Ȱ��ȭ ���� ������Ʈ

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

	// 1. ��θ� '/'�� ����
	std::vector<std::wstring> tokens = WStringHelper::Split(path, L'/');
	if (tokens.empty())
		return nullptr;

	// 2. ���� Transform���� ����
	Transform* current = this;

	// 3. ��ū ��ȸ
	for (const auto& token : tokens)
	{
		bool found = false;

		for (Transform* child : current->m_childs)
		{
			if (child && child->name == token)
			{
				// ��ġ�ϴ� �ڽ� �߰�
				current = child;
				found = true;
				break;
			}
		}

		if (!found)
		{
			// ���� �������� �̸��� �� ã���� ��� ����
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
		// �θ� ������ �θ��� ȸ���� ���� ���� ȸ�� ���
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
		// ���� �������� �̵�
		Vector2 worldPos = GetPosition() + translation;
		SetPosition(worldPos);
	}
	else
	{
		// ���� �������� �̵� (ȸ�� ����)
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
	// TransformDirection�� ����ȯ�Դϴ�.
	float worldRotation = -GetRotation() * Mathf::Deg2Rad; // ��ȸ��
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
			m_localRotation * Mathf::Deg2Rad, // degree�� radian���� ��ȯ
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