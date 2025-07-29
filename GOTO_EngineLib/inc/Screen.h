#pragma once
#include "RenderManager.h"
#include "Rect.h"
#include "Vector2.h"

namespace GOTOEngine
{
	class Screen
	{
	public:
		/// <summary>
		/// ȭ���� �ʺ� ��ȯ�մϴ�.
		/// </summary>
		static float GetWidth()
		{
			return RenderManager::Get()->GetTargetWidth();
		}
		/// <summary>
		/// ȭ���� ���̸� ��ȯ�մϴ�.
		/// </summary>
		static float GetHeight()
		{
			return RenderManager::Get()->GetTargetHeight();
		}
		/// <summary>
		/// ȭ���� ũ�⸦ ��ȯ�մϴ�.
		/// </summary>
		static Vector2 GetSize()
		{
			return Vector2(GetWidth(), GetHeight());
		}

		static void SetSize(float width, float height)
		{
			RenderManager::Get()->SetTargetWidth(width);
			RenderManager::Get()->SetTargetHeight(height);
		}
	};
}