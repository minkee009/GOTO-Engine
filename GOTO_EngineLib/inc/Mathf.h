#pragma once
#include "BBox.h"

namespace GOTOEngine
{
	class Mathf
	{
	public:

		static float Lerp(float a, float b, float t);
		static float Deg2Rad;
		static float Rad2Deg;
		static float PI;
		static float NormalizeAngle(float angle);
	};
}
