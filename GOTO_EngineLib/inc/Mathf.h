#pragma once
#include "BBox.h"
#include <array>

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
		static bool Approximately(float a, float b);

		// ���� PerlinNoise �Լ�
		static float PerlinNoise(float x, float y);
		static float Clamp(float value, float min, float max);

	private:
		static std::array<int, 512> p;
		static void InitializePermutation();

		static float Fade(float t);
		static float LerpInternal(float a, float b, float t);
		static float Grad(int hash, float x, float y);
		static bool permutationInitialized;
	};
}