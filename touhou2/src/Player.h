#pragma once
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>

// NOTE: switch to xspeed/yspeed or get rid of speed completely?
struct Player
{
public:
	// player definitions
	constexpr static int powerMax = 128;

	constexpr static float moveSpeed = 4.0f;
	constexpr static float focusSpeed = 2.0f;

	constexpr static float startX = 384 / 2;
	constexpr static float startY = 448 / 4 * 3;
	constexpr static float appearX = 384 / 2;
	constexpr static float appearY = 448;

	constexpr static float fireTime = 10.0f;
	constexpr static float deathbombTime = 15.0f;
	constexpr static float appearTime = 60.0f;

public:
	// standard methods
	Player();

	void update(float delta);
	void physicsUpdate(float delta);
	void endUpdate(float delta);
	void draw(sf::RenderTexture& target, float delta) const;

public:
	// standard fields
	bool dead = false;
	float x = 0.0f;
	float y = 0.0f;
	float speed = 0.0f;
	float direction = 0.0f;
	float radius = 0.0f;

public:
	// player methods
	void getHit();

public:
	// player fields
	int score = 0;
	int lives = 5;
	int bombs = 3;
	int power = 0;
	int graze = 0;
	int point = 0;

	bool focus = false;
	float invincibility = 0.0f;

	float fireTimer = 0.0f;
	float deathbombTimer = 0.0f;
	float appearTimer = 0.0f;

public:
	// states
	using State = void(Player::*)(float);

	void normalState(float delta);
	void dyingState(float delta);
	void appearingState(float delta);

	void setNormalState();
	void setDyingState();
	void setAppearingState();

	State state = nullptr;

public:
	// other
	static void luaRegister(luabridge::Namespace nameSpace);

	float t = 0.0f;
	float a = 0.0f;
};
