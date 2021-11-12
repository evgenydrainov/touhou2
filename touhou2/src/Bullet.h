#pragma once
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>

struct Bullet
{
public:
	Bullet(float x, float y, float speed, float direction, float radius);

	// standard methods
	void update(float delta);
	void physicsUpdate(float delta);
	void endUpdate(float delta);
	
public:
	// standard fields
	bool dead = false;
	float x = 0.0f;
	float y = 0.0f;
	float speed = 0.0f;
	float direction = 0.0f;
	float acc = 0.0f;
	float speedMin = 0.0f;
	float radius = 0.0f;

public:
	// bullet methods


public:
	// bullet fields
	

public:
	// other
	
};
