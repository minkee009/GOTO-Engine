#include "Camera.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "RenderManager.h"
#include "IWindow.h"

GOTOEngine::Camera* GOTOEngine::Camera::s_mainCam = nullptr;

GOTOEngine::Camera::Camera() : m_depth(0), m_size(1.0f),m_renderLayer(static_cast<size_t>(-1))
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
	//Screen Ŭ���� �ۼ��ϱ� ��
	auto mat = GetGameObject()->GetTransform()->GetLocalMatrix().Inverse();
	mat = Matrix4x4::Translate(RenderManager::Get()->GetWindow()->GetWidth() * 0.5f, RenderManager::Get()->GetWindow()->GetHeight() * 0.5f, 0) * Matrix4x4::Scale(1.0f * m_size, -1.0f * m_size, 1.0f * m_size) * mat;
	
	return mat;
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

