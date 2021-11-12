#pragma once
#include <SFML/Graphics.hpp>

struct Pickup
{
public:
	// pickup definitions
	enum class Type
	{
		Null,
		Point,
		Power
	};

public:
	// standard methods


public:
	// standrard fields
	float x = 0.0f;
	float y = 0.0f;

public:
	// pickup methods


public:
	// pickup fields
	Type type{};
};
