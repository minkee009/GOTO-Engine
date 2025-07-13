#pragma once

namespace GOTOEngine
{
	struct Rect
	{
		float x;
		float y;
		float width;
		float height;

		Rect(float x, float y, 
			float width, float height) 
			: x(x) 
			, y(y) 
			, width(width)
			, height(height) 
		{
		}
	};
}