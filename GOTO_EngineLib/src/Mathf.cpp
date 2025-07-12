#include "Mathf.h"
#include <algorithm>
#include <cmath>

using namespace GOTOEngine;

float Mathf::Lerp(float a, float b, float t)
{
	t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
	return a + (b - a) * t;


}

float GOTOEngine::Mathf::NormalizeAngle(float angle)
{
	angle = std::fmod(angle, 360.0f);
	if (angle < 0.0f)
		angle += 360.0f;
	return angle;
}

bool GOTOEngine::Mathf::Approximately(float a, float b)
{
	static constexpr float epsilon = std::numeric_limits<float>::epsilon();
	float diff = std::abs(a - b);

	if (diff < epsilon)
		return true;

	return diff < std::max(std::abs(a), std::abs(b)) * epsilon * 8.0f;
}

float Mathf::PI = 3.14159265358979323846f;
float Mathf::Deg2Rad = 3.1415926535f / 180.0f;
float Mathf::Rad2Deg = 180.0f / 3.1415926535f;
