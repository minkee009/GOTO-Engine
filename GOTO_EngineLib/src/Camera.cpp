#include "Camera.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "RenderManager.h"
#include "IWindow.h"

GOTOEngine::Camera* GOTOEngine::Camera::s_mainCam = nullptr;

GOTOEngine::Camera::Camera() : m_depth(0), m_size(1.0f), m_rect({0.0f,0.0f,1.0f,1.0f}), m_renderLayer(static_cast<size_t>(-1))
{
	RegisterMessage("OnEnable", &Camera::OnEnable);
	RegisterMessage("OnDisable", &Camera::OnDisable);
	RegisterMessage("OnDestroy", &Camera::OnDestroy);

	RenderManager::Get()->RegisterCamera(this);
}

GOTOEngine::Camera::~Camera()
{
	RenderManager::Get()->UnRegisterCamera(this);
}

void GOTOEngine::Camera::OnEnable()
{

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

GOTOEngine::Matrix4x4 GOTOEngine::Camera::GetMatrix()
{
	auto mat = Matrix4x4::Scale(m_size, m_size, 1) * GetGameObject()->GetTransform()->GetLocalMatrix();

	return mat.Inverse();
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

