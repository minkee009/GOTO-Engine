#pragma once
#include "Resource.h"
#include "RenderManager.h"
#include "IRenderBitmap.h"
#include "Rect.h"

namespace GOTOEngine
{
	enum class TextureRenderMode
	{
		Point,         // �� ��Ʈ��: ���� ����, �� �ȼ� (Nearest)
		Bilinear,      // ������: 2D ���� (�⺻)
		//Trilinear,     // Ʈ������: mipmap ���� (3D�� �ַ�)
		//Crisp,         // �ѷ��ѵ� ���ػ� ���� (�߰� �ɼ�)
		//Custom         // Ŀ���� ���̴� �� ����� ���� ���
	};

	class Texture2D : public Resource
	{
	private:
		friend class ResourceManager;
		IRenderBitmap* m_bitmapHandle;
		Rect m_rect;
		TextureRenderMode m_renderMode;
		void LoadFromFilePath(const std::wstring& filePath) override;
		~Texture2D();
	public:
		Texture2D();

		void SetRenderMode(TextureRenderMode mode) { m_renderMode = mode; }
		const TextureRenderMode& GetRenderMode() { return m_renderMode; }

		const IRenderBitmap* GetBitmap() const { return m_bitmapHandle; }
		const Rect& GetRect() const { return m_rect; }
		bool IsValidData() override { return m_bitmapHandle; }
	};
}