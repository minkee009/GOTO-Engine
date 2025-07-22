#include "Camera.h"
#include "Transform.h"
#include "RenderManager.h"
#include "IWindow.h"
#include "TimeManager.h"
#include "IRenderAPI.h"

GOTOEngine::Camera* GOTOEngine::Camera::s_mainCam = nullptr;


GOTOEngine::Camera::Camera() : m_depth(0), m_size(1.0f), m_rect(Rect{ 0.0f,0.0f,1.0f,1.0f }), m_backGroundColor(Color{ 0,0,0,255 }/* black */)
, m_renderLayer(static_cast<size_t>(-1)) , m_isMatrixDirty(true), m_lastPosition(0.0f, 0.0f), m_lastRotation(0.0f)
{
	REGISTER_BEHAVIOUR_MESSAGE(OnEnable);
	REGISTER_BEHAVIOUR_MESSAGE(OnDisable);
	REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);

	m_cachedMatrix.SetIdentity();
	RenderManager::Get()->RegisterCamera(this);
}

GOTOEngine::Camera::~Camera()
{
	RenderManager::Get()->UnRegisterCamera(this);
}

void GOTOEngine::Camera::OnEnable()
{
	m_isMatrixDirty = true;
}

void GOTOEngine::Camera::OnDisable()
{

}

void GOTOEngine::Camera::OnDestroy()
{
	if (s_mainCam == this)
		s_mainCam = nullptr;
}

void GOTOEngine::Camera::SetDepth(int value)
{
	RenderManager::Get()->SetCamSortDirty(); m_depth = value;
}

GOTOEngine::Matrix3x3 GOTOEngine::Camera::GetMatrix()
{
	Transform* transform = GetGameObject()->GetTransform();

	// Transform�� WorldMatrix�� dirty���� Ȯ�� (�� ȿ����)
	// �Ǵ� �ܼ��� �� �񱳷� ���� ����
	Vector2 currentPosition = transform->GetPosition();
	float currentRotation = transform->GetRotation();

	// Transform�� ����Ǿ��ų� ����� ����� ��쿡�� ����
	if (m_isMatrixDirty ||
		currentPosition != m_lastPosition ||
		currentRotation != m_lastRotation )
	{
		m_cachedMatrix = transform->GetWorldMatrix().Inverse();
		auto currentLossyScale = transform->GetLossyScale();
		m_cachedMatrix = Matrix3x3::Scale(currentLossyScale.x * m_size, currentLossyScale.y * m_size) * m_cachedMatrix;

		// ĳ�� ���� ������Ʈ
		m_lastPosition = currentPosition;
		m_lastRotation = currentRotation;
		m_isMatrixDirty = false;
	}

	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	return Matrix3x3::Translate(renderAPI->GetWindow().GetWidth() * (m_rect.x + (m_rect.width * 0.5f)),
		renderAPI->GetWindow().GetHeight() * (1.0f - m_rect.y - (m_rect.height * 0.5f)))
		* Matrix3x3::Scale(1.0f, -1.0f) * m_cachedMatrix;
}

GOTOEngine::Camera* GOTOEngine::Camera::GetMainCamera()
{
	if (!s_mainCam) 
	{
		//���ο� ����ķ ã��
		auto mainCamGOs = GameObject::FindGameObjectsWithTag("MainCamera");

		if (mainCamGOs.size() != 0)
			for (auto& camGO : mainCamGOs)
			{
				if (auto mainCam = camGO->GetComponent<Camera>())
				{
					s_mainCam = mainCam;
					break;
				}
			}
	}

	return s_mainCam;
}

GOTOEngine::GameObject* GOTOEngine::Camera::CreateMainCamera()
{
	//ī�޶� ������Ʈ�� ������ ���� ����
	if (!SceneManager::Get()->GetCurrentScene())
		return nullptr;

	auto cameraGO = new GameObject(L"MainCamera");
	cameraGO->SetTag("MainCamera");
	auto cam = cameraGO->AddComponent<Camera>();

	if (!s_mainCam)
		s_mainCam = cam;

	return cameraGO;
}



GOTOEngine::Vector2 GOTOEngine::Camera::ScreenToViewportPoint(const Vector2& screenPoint)
{
	// 1. ������ API�� ������ ũ�⸦ �����ɴϴ�.
	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float windowWidth = static_cast<float>(renderAPI->GetWindow().GetWidth());
	float windowHeight = static_cast<float>(renderAPI->GetWindow().GetHeight());

	// 2. ����Ʈ�� �ȼ� ��ǥ �� ũ�⸦ ����մϴ�.
	// m_rect�� (x, y, width, height)�� ����ȭ�� �� (0.0f ~ 1.0f)�̸�,
	// GOTOEngine�� ȭ�� ��ǥ�� (0,0)�� ���� �ϴ��Դϴ�.
	float viewportPixelX = windowWidth * m_rect.x;
	float viewportPixelY = windowHeight * m_rect.y; // ����Ʈ�� �ϴ� Y �ȼ� ��ǥ (ȭ�� �ϴ� ����)
	float viewportPixelWidth = windowWidth * m_rect.width;
	float viewportPixelHeight = windowHeight * m_rect.height;

	// 3. �Էµ� screenPoint (ȭ�� ���� �ϴ� 0,0 ����)�� ����Ʈ�� �ȼ� �������� ��ȯ�մϴ�.
	// screenPoint.x�� screenPoint.y�� �̹� GOTOEngine�� ��ǥ�迡 ������ �����Ƿ�
	// ����Ʈ�� ���� �ϴ� �ȼ� ��ǥ�� �������� ������� ��ġ�� ����մϴ�.
	float relativeX = screenPoint.x - viewportPixelX;
	float relativeY = screenPoint.y - viewportPixelY; // ����Ʈ �ϴ� ���� ��� Y

	// 4. ��� �ȼ� ��ǥ�� ����Ʈ�� ����ȭ�� ��ǥ�� ��ȯ�մϴ�.
	GOTOEngine::Vector2 viewportPoint;

	if (viewportPixelWidth != 0.0f)
	{
		viewportPoint.x = relativeX / viewportPixelWidth;
	}
	else
	{
		viewportPoint.x = 0.0f; // ����Ʈ �ʺ� 0�̸� 0���� ó��
	}

	if (viewportPixelHeight != 0.0f)
	{
		// GOTOEngine�� Y��� ����Ʈ ��ǥ���� Y�� ������ ��ġ�ϹǷ� Y���� ������ �ʿ䰡 �����ϴ�.
		viewportPoint.y = relativeY / viewportPixelHeight;
	}
	else
	{
		viewportPoint.y = 0.0f; // ����Ʈ ���̰� 0�̸� 0���� ó��
	}

	return viewportPoint;
}
GOTOEngine::Vector2 GOTOEngine::Camera::ScreenToWorldPoint(const Vector2& screenPoint)
{
	Transform* transform = GetGameObject()->GetTransform();
	auto currentLossyScale = transform->GetLossyScale();

	// �� ��� (World �� Camera space) ����
	Matrix3x3 viewMatrix = transform->GetWorldMatrix().Inverse();

	// ��ü ������ ���� (m_size ����)
	float sx = currentLossyScale.x * m_size;
	float sy = currentLossyScale.y * m_size;

	// ī�޶� �߽��� �����ϴ� ��ũ�� ���� ��ġ (�ȼ� ��ǥ)
	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float screenCenterX = renderAPI->GetWindow().GetWidth() * (m_rect.x + m_rect.width * 0.5f);
	float screenCenterY = renderAPI->GetWindow().GetHeight() * (m_rect.y + m_rect.height * 0.5f);

	// ���� ��ȯ: ȭ�� ��ǥ �� ���� ��ǥ
	// step 1: ȭ�� ������ ����
	Vector2 centeredScreenPoint = screenPoint - Vector2(screenCenterX, screenCenterY);

	// step 2: ������ ����ȯ
	centeredScreenPoint.x /= sx;
	centeredScreenPoint.y /= sy;

	// step 3: �� ����ȯ ����
	return viewMatrix.MultiplyPoint(centeredScreenPoint);
}

GOTOEngine::Vector2 GOTOEngine::Camera::WorldToViewportPoint(const Vector2& worldPoint)
{
	Transform* transform = GetGameObject()->GetTransform();
	auto currentLossyScale = transform->GetLossyScale();

	Matrix3x3 viewMatrix = transform->GetWorldMatrix().Inverse();
	GOTOEngine::Vector2 cameraSpacePoint = viewMatrix.MultiplyPoint(worldPoint);

	float sx = currentLossyScale.x * m_size;
	float sy = currentLossyScale.y * m_size;
	GOTOEngine::Vector2 scaledCameraSpacePoint = Vector2(cameraSpacePoint.x * sx, cameraSpacePoint.y * sy);

	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float windowWidth = static_cast<float>(renderAPI->GetWindow().GetWidth());
	float windowHeight = static_cast<float>(renderAPI->GetWindow().GetHeight());

	float screenCenterX = windowWidth * (m_rect.x + m_rect.width * 0.5f);
	float screenCenterY = windowHeight * (m_rect.y + m_rect.height * 0.5f);

	GOTOEngine::Vector2 screenPixelPoint = Vector2(
		scaledCameraSpacePoint.x + screenCenterX,
		scaledCameraSpacePoint.y + screenCenterY
	);

	float viewportPixelX = windowWidth * m_rect.x;
	float viewportPixelY = windowHeight * m_rect.y; // ����Ʈ�� �ϴ� Y �ȼ� ��ǥ (ȭ�� �ϴ� ����)
	float viewportPixelWidth = windowWidth * m_rect.width;
	float viewportPixelHeight = windowHeight * m_rect.height;

	float relativeX = screenPixelPoint.x - viewportPixelX;
	float relativeY = screenPixelPoint.y - viewportPixelY;

	GOTOEngine::Vector2 viewportPoint;

	if (viewportPixelWidth != 0.0f)
	{
		viewportPoint.x = relativeX / viewportPixelWidth;
	}
	else
	{
		viewportPoint.x = 0.0f; // ����Ʈ �ʺ� 0�̸� 0���� ó��
	}

	if (viewportPixelHeight != 0.0f)
	{
		viewportPoint.y = relativeY / viewportPixelHeight;
	}
	else
	{
		viewportPoint.y = 0.0f; // ����Ʈ ���̰� 0�̸� 0���� ó��
	}

	return viewportPoint;
}

GOTOEngine::Vector2 GOTOEngine::Camera::WorldToScreenPoint(const Vector2& worldPoint)
{
	Transform* transform = GetGameObject()->GetTransform();
	auto currentLossyScale = transform->GetLossyScale();

	// 1. ���� ����Ʈ�� ī�޶� ����(Camera Local Space)���� ��ȯ�մϴ�.
	// �̴� ī�޶��� ���� ����� ������� ���ϴ� �Ͱ� �����ϴ�.
	Matrix3x3 viewMatrix = transform->GetWorldMatrix().Inverse();
	GOTOEngine::Vector2 cameraSpacePoint = viewMatrix.MultiplyPoint(worldPoint);

	// 2. ī�޶��� ������(m_size �� lossyScale)�� �����Ͽ� ȭ�� ���� �ȼ� �����Ͽ� ����ϴ�.
	// ScreenToWorldPoint���� �������� �����Ƿ� ���⼭�� ������ �մϴ�.
	float sx = currentLossyScale.x * m_size;
	float sy = currentLossyScale.y * m_size;
	GOTOEngine::Vector2 scaledCameraSpacePoint = Vector2(cameraSpacePoint.x * sx, cameraSpacePoint.y * sy);

	// 3. ī�޶� �߽��� �����ϴ� ��ũ�� ���� ��ġ(�ȼ� ��ǥ)�� �����ɴϴ�.
	// �� ��ǥ�� GOTOEngine�� ȭ�� ��ǥ�� (���� �ϴ� 0,0 ����)�� �����ϴ�.
	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float windowWidth = static_cast<float>(renderAPI->GetWindow().GetWidth());
	float windowHeight = static_cast<float>(renderAPI->GetWindow().GetHeight());

	float screenCenterX = windowWidth * (m_rect.x + m_rect.width * 0.5f);
	float screenCenterY = windowHeight * (m_rect.y + m_rect.height * 0.5f);

	// 4. �������� ����� ī�޶� ���� ����Ʈ�� ȭ�� �ȼ� ��ǥ(���� �ϴ� 0,0 ����)�� ��ȯ�մϴ�.
	// ScreenToWorldPoint���� ������ �����Ƿ� ���⼭�� ������ �մϴ�.
	GOTOEngine::Vector2 screenPixelPoint = Vector2(
		scaledCameraSpacePoint.x + screenCenterX,
		scaledCameraSpacePoint.y + screenCenterY
	);

	return screenPixelPoint;
}

GOTOEngine::Vector2 GOTOEngine::Camera::ViewportToScreenPoint(const Vector2& viewportPoint)
{
	// 1. ������ API�� ������ ũ�⸦ �����ɴϴ�.
	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float windowWidth = static_cast<float>(renderAPI->GetWindow().GetWidth());
	float windowHeight = static_cast<float>(renderAPI->GetWindow().GetHeight());

	// 2. ����Ʈ�� �ȼ� ��ǥ �� ũ�⸦ ����մϴ�.
	// m_rect�� (x, y, width, height)�� ����ȭ�� �� (0.0f ~ 1.0f)�̸�,
	// GOTOEngine�� ȭ�� ��ǥ�� (0,0)�� ���� �ϴ��Դϴ�.
	float viewportPixelX = windowWidth * m_rect.x;
	float viewportPixelY = windowHeight * m_rect.y; // ����Ʈ�� �ϴ� Y �ȼ� ��ǥ (ȭ�� �ϴ� ����)
	float viewportPixelWidth = windowWidth * m_rect.width;
	float viewportPixelHeight = windowHeight * m_rect.height;

	// 3. ����Ʈ�� ����ȭ�� ��ǥ�� ����Ʈ ���� ������� �ȼ� ��ǥ�� ��ȯ�մϴ�.
	float relativePixelX = viewportPoint.x * viewportPixelWidth;
	float relativePixelY = viewportPoint.y * viewportPixelHeight;

	// 4. ��� �ȼ� ��ǥ�� ����Ʈ�� ���� �ȼ� ��ǥ�� ���Ͽ� ���� ȭ�� �ȼ� ��ǥ�� ����ϴ�.
	GOTOEngine::Vector2 screenPixelPoint = Vector2(
		relativePixelX + viewportPixelX,
		relativePixelY + viewportPixelY
	);

	return screenPixelPoint;
}

GOTOEngine::Vector2 GOTOEngine::Camera::ViewportToWorldPoint(const Vector2& viewportPoint)
{
	// 1. ����Ʈ ��ǥ�� ȭ�� �ȼ� ��ǥ�� ��ȯ�մϴ�. (ViewportToScreenPoint �Լ� ����)
	GOTOEngine::Vector2 screenPixelPoint = ViewportToScreenPoint(viewportPoint);

	// 2. ScreenToWorldPoint ������ ������ �����Ͽ� ȭ�� �ȼ� ��ǥ�� ���� ��ǥ�� ��ȯ�մϴ�.
	Transform* transform = GetGameObject()->GetTransform();
	auto currentLossyScale = transform->GetLossyScale();

	// ī�޶� �߽��� �����ϴ� ��ũ�� ���� ��ġ (�ȼ� ��ǥ)
	auto renderAPI = RenderManager::Get()->GetRenderAPI();
	float windowWidth = static_cast<float>(renderAPI->GetWindow().GetWidth());
	float windowHeight = static_cast<float>(renderAPI->GetWindow().GetHeight());

	float screenCenterX = windowWidth * (m_rect.x + m_rect.width * 0.5f);
	float screenCenterY = windowHeight * (m_rect.y + m_rect.height * 0.5f);

	// step 1: ȭ�� ������ ���� (screenPixelPoint���� screenCenter�� ���� �߾��� 0,0���� ����ϴ�)
	Vector2 centeredScreenPoint = screenPixelPoint - Vector2(screenCenterX, screenCenterY);

	// step 2: ������ ����ȯ (ScreenToWorldPoint���� �ߴ� �״�� �������� �մϴ�)
	// 0���� ������ ���� �����ϱ� ���� ���� ��ġ �߰�
	float sx = currentLossyScale.x * m_size;
	float sy = currentLossyScale.y * m_size;

	if (sx != 0.0f) {
		centeredScreenPoint.x /= sx;
	}
	else {
		centeredScreenPoint.x = 0.0f; // ������ X�� 0�̸� 0���� ó��
	}
	if (sy != 0.0f) {
		centeredScreenPoint.y /= sy;
	}
	else {
		centeredScreenPoint.y = 0.0f; // ������ Y�� 0�̸� 0���� ó��
	}

	// step 3: �� ����ȯ ����
	// (World �� Camera space) ����� ������� (Camera space �� World) ����Դϴ�.
	Matrix3x3 viewMatrixInverse = transform->GetWorldMatrix(); // GetWorldMatrix()�� World -> Camera Inverse�� �̹� ����Ǿ� ����
	// �ƴ�, ScreenToWorldPoint���� viewMatrix = transform->GetWorldMatrix().Inverse(); �����ϴ�.
	// �̴� World -> Camera space ��ȯ�Դϴ�.
	// �츮�� Camera space -> World space�� �����ϹǷ�, transform->GetWorldMatrix()�� �ʿ��մϴ�.
	Matrix3x3 cameraToWorldMatrix = transform->GetWorldMatrix();


	return cameraToWorldMatrix.MultiplyPoint(centeredScreenPoint);
}