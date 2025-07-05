#pragma once
#include <IRenderImage.h>
#include <string>
#include <vector>
#include <Windows.h>

namespace GOTOEngine
{
	class D2DImage : public IRenderImage
	{
	public:
		D2DImage(const std::wstring& filePath, int pivotX = 0, int pivotY = 0); //���� �̹��� ����
		D2DImage(IRenderImage* imageAtlas, int partialX, int partialY, int width, int height, int pivotX = 0, int pivotY = 0); //�κ� �̹��� ����
		~D2DImage();

		// IImage��(��) ���� ��ӵ�
		const std::wstring& GetFilePath() const override { return m_filePath; }
		void* GetNativeHandle() const override { return m_image; }
		int GetWidth() const override { return m_width; }
		int GetHeight() const override { return m_height; }

		//���� �̹��� ũ��
		int GetSrcWidth() const { return m_srcWidth; }
		int GetSrcHeight() const { return m_srcHeight; }

		//�κ� �̹��� ����
		int IsPartial() const { return m_isPartial; }

		int GetPartialX() const { return m_partialX; }
		int GetPartialY() const { return m_partialY; }

	private:
		std::vector<BYTE>* m_image = nullptr;
		const std::wstring m_filePath;
		int m_partialX = 0;
		int m_partialY = 0;
		int m_srcWidth = 0;
		int m_srcHeight = 0;
		int m_width = 0;
		int m_height = 0;
		bool m_isPartial = false;
	};
}
