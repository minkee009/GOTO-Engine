#pragma once

#define BBOX_FLAG_PUSHLEFT 1
#define BBOX_FLAG_PUSHRIGHT 2
#define BBOX_FLAG_PUSHUP 4
#define BBOX_FLAG_PUSHDOWN 8

namespace GOTOEngine
{
	struct BBox
	{
		float minX;
		float minY;
		float maxX;
		float maxY;

		BBox() : minX(0), minY(0), maxX(0), maxY(0) {}

		BBox(float minX, float minY, float maxX, float maxY)
			: minX(minX), minY(minY), maxX(maxX), maxY(maxY)
		{
		}
		bool Intersect(BBox other);
		bool Intersect(float pointX, float pointY);
		int ApplyPenetration(float* posX, float* posY, BBox b);
		int CalcPenetration(BBox other);
	};
}
