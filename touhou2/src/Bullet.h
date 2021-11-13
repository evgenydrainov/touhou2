#pragma once
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>

struct Bullet
{
public:
	Bullet(float x, float y, float speed, float direction, float radius, lua_State* L);
	Bullet(float x, float y, float speed, float direction, float radius, luabridge::LuaRef script, lua_State* L);

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
	luabridge::LuaRef co;
	float co_timer = 0.0f;
	bool co_running = false;

public:
	// other
	static void luaRegister(luabridge::Namespace nameSpace);
};
