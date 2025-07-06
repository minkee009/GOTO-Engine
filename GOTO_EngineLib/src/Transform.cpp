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
	: m_localPosition(0.0f, 0.0f, 0.0f)
	, m_localRotation(0.0f, 0.0f, 0.0f, 1.0f)
	, m_localScale(1.0f, 1.0f, 1.0f)
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

	//�ڽ��� �ݺ��� ��ġ�� ã��
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

const GOTOEngine::Vector3 GOTOEngine::Transform::GetPosition() const
{
	if (m_parent)
	{
		// �θ� ���� ����� Ȱ��
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
		// �θ� �ִٸ� �θ��� ȸ���� ����
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
		// �θ� �ִٸ� �θ��� ȸ���� ����
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
	SetParent(parent, true); // ���� �������� �����ϸ鼭 �θ� ����
}

void GOTOEngine::Transform::SetParent(Transform* parent, bool worldPositionStays)
{
	// �̹� ���� �θ�� return
	if (m_parent == parent) return;

	// ���� ���� ��� ���
	Matrix4x4 worldMatrixBefore = GetWorldMatrix();

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
			Matrix4x4 parentWorldInv = m_parent->GetWorldMatrix().Inverse();
			Matrix4x4 newLocal = parentWorldInv * worldMatrixBefore;

			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetLossyScale();
		}
		else
		{
			// �θ� ������ ���� = ����
			Matrix4x4 newLocal = worldMatrixBefore;
			m_localPosition = newLocal.GetPosition();
			m_localRotation = newLocal.GetRotation();
			m_localScale = newLocal.GetLossyScale();
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

	// ȸ�� ����� ���ʹϾ����� ��ȯ
	Matrix4x4 rotMat;
	rotMat.SetIdentity();

	// �� �켱 ����
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
	// ���� ���� ��ġ
	Vector3 currentWorldPosition = GetPosition();

	// ȸ�� �߽������κ��� ���� ��ġ������ ����
	Vector3 offset = currentWorldPosition - point;

	// ȸ�� ���ʹϾ� ����
	Quaternion rotation = Quaternion::AngleAxis(angle, axis.Normalized());

	// offset ���͸� ȸ��
	Vector3 rotatedOffset = rotation * offset;

	// ���ο� ���� ��ġ ���
	Vector3 newWorldPosition = point + rotatedOffset;

	// ���� Transform�� �θ� ������ �ִٸ�, �� ���� ��ġ�� ���� �������� ��ȯ�ؾ� �մϴ�.
	if (m_parent)
	{
		// �θ��� ������� ����Ͽ� ���� ��ġ�� ���� ��ġ�� ��ȯ
		Matrix4x4 parentWorldMatrix = m_parent->GetWorldMatrix();
		Matrix4x4 parentInverseWorldMatrix = parentWorldMatrix.Inverse(); // �θ� ���� ����� �����

		m_localPosition = parentInverseWorldMatrix.MultiplyPoint(newWorldPosition);
	}
	else
	{
		m_localPosition = newWorldPosition;
	}

	// ���� ���� ȸ�� (normalized �� ����)
	Quaternion currentWorldRotation = GetRotation().Normalized();

	// ������ ȸ�� ���ʹϾ�
	Quaternion addRotation = Quaternion::AngleAxis(angle, axis.Normalized());

	// ���ο� ���� ȸ��
	Quaternion newWorldRotation = addRotation * currentWorldRotation; // ȸ�� ������ ����

	if (m_parent)
	{
		// �θ��� ���� ȸ���� ����ȯ�� ����Ͽ� ���ο� ���� ȸ�� ���
		Quaternion parentWorldRotationInverse = m_parent->GetRotation().Inverse();
		m_localRotation = parentWorldRotationInverse * newWorldRotation;
	}
	else
	{
		m_localRotation = newWorldRotation;
	}

	MarkDirty(); // ��ġ�� ȸ���� ����Ǿ����Ƿ� ����� �����ٰ� ǥ��
}

GOTOEngine::Vector3 GOTOEngine::Transform::TransformDirection(const Vector3& direction) const
{
	Quaternion worldRotation = GetRotation(); // ���� ȸ�� ���ʹϾ�

	return worldRotation * direction;
}

GOTOEngine::Vector3 GOTOEngine::Transform::InverseTransformDirection(const Vector3& direction) const
{
	// TransformDirection�� ����ȯ�Դϴ�.
	// ���� ȸ���� �ӷ� ���Ҽ�(����ȯ)�� ����մϴ�.
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
