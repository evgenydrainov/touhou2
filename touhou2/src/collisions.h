#pragma once
#include "mymath.h"

namespace col
{
	constexpr bool circle_vs_circle(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		return (math::sqr(x1 - x2) + math::sqr(y1 - y2)) < math::sqr(r1 + r2);
	}
}
