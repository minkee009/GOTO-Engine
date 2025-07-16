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

	// Transform의 WorldMatrix가 dirty한지 확인 (더 효율적)
	// 또는 단순히 값 비교로 변경 감지
	Vector2 currentPosition = transform->GetPosition();
	float currentRotation = transform->GetRotation();

	// Transform이 변경되었거나 사이즈가 변경된 경우에만 재계산
	if (m_isMatrixDirty ||
		currentPosition != m_lastPosition ||
		currentRotation != m_lastRotation )
	{
		m_cachedMatrix = transform->GetWorldMatrix().Inverse();
		auto currentLossyScale = transform->GetLossyScale();
		m_cachedMatrix = Matrix3x3::Scale(currentLossyScale.x * m_size, currentLossyScale.y * m_size) * m_cachedMatrix;

		// 캐시 상태 업데이트
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
		//새로운 메인캠 찾기
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
	//카메라 오브젝트를 생성할 씬이 없음
	if (!SceneManager::Get()->GetCurrentScene())
		return nullptr;

	auto cameraGO = new GameObject(L"MainCamera");
	cameraGO->SetTag("MainCamera");
	auto cam = cameraGO->AddComponent<Camera>();

	if (!s_mainCam)
		s_mainCam = cam;

	return cameraGO;
}

