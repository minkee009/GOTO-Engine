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
		D2DImage(const std::wstring& filePath); //���� �̹��� ����
		~D2DImage();

		// IImage��(��) ���� ��ӵ�
		const std::wstring& GetFilePath() const override { return m_filePath; }
		void* GetNativeHandle() const override { return m_image; }
		int GetWidth() const override { return m_width; }
		int GetHeight() const override { return m_height; }

	private:
		std::vector<BYTE>* m_image = nullptr;
		const std::wstring m_filePath;
		int m_width = 0;
		int m_height = 0;
	};
}
