#pragma once
#include <cstdint>

namespace GOTOEngine
{
	struct Color
	{
		uint8_t R;
		uint8_t G;
		uint8_t B;
		uint8_t A;
		Color() :R(0), G(0), B(0), A(0) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) {}
	};
}
