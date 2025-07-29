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
		/// 화면의 너비를 반환합니다.
		/// </summary>
		static float GetWidth()
		{
			return RenderManager::Get()->GetTargetWidth();
		}
		/// <summary>
		/// 화면의 높이를 반환합니다.
		/// </summary>
		static float GetHeight()
		{
			return RenderManager::Get()->GetTargetHeight();
		}
		/// <summary>
		/// 화면의 크기를 반환합니다.
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