#pragma once
#include "sine_table.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace math
{
	constexpr float pi = 3.14159265358979323846f;

	template <typename T>
	constexpr T approach(T start, T end, T shift) { return start + std::clamp(end - start, -shift, shift); }

	template <typename T>
	constexpr T modulo(T x, T n) { return (x % n + n) % n; }

	template <typename T>
	constexpr T sqr(T x) { return x * x; }

	template <typename T>
	constexpr T floor_to(T x, T n) { return std::floor(x / n) * n; }

	template <typename T>
	constexpr T round_to(T x, T n) { return std::round(x / n) * n; }

	template <typename T>
	constexpr T ceil_to(T x, T n) { return std::ceil(x / n) * n; }
	
	template <typename T>
	constexpr T lerp(T a, T b, T t) { return a + (b - a) * t; }

	constexpr float sin(float degrees) { return sine_table[modulo((int)degrees, 360)]; }
	constexpr float cos(float degrees) { return sine_table[modulo((int)degrees + 90, 360)]; }

	constexpr float deg2rad(float degrees) { return degrees * pi / 180.0f; }
	constexpr float rad2deg(float radians) { return radians * 180.0f / pi; }

	inline float point_direction(float x1, float y1, float x2, float y2) { return rad2deg(std::atan2(y1 - y2, x2 - x1)); }
	inline float point_distance(float x1, float y1, float x2, float y2) { return std::sqrt(sqr(x2 - x1) + sqr(y2 - y1)); }

	inline float random(float a, float b)
	{
		constexpr float max = std::mt19937::max();
		static std::mt19937 engine{ std::random_device()() };
		float r = (float)engine() / max;
		float range = std::abs(a - b);
		return std::min(a, b) + r * range;
	}
}
