#pragma once
#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

class Player : public GameObject
{
public:
	Player();

	void update(float delta);
	void checkCollisions();
	void checkBounds();
	void animate(float delta);
	void draw(sf::RenderTexture& target, float delta) const;

	static void luaRegister(luabridge::Namespace nameSpace);

	inline int getScore() const { return m_score; }
	inline int getLives() const { return m_lives; }
	inline int getBombs() const { return m_bombs; }
	inline int getPower() const { return m_power; }
	inline int getGraze() const { return m_graze; }
	inline int getPoint() const { return m_point; }
	inline int getPointNext() const { return 50; }
	inline int getPowerMax() const { return m_powerMax; }

private:
	constexpr static float m_moveSpeed = 4.0f;
	constexpr static float m_focusSpeed = 2.0f;

	constexpr static float m_startX = 384 / 2;
	constexpr static float m_startY = 448 / 4 * 3;
	constexpr static float m_appearX = 384 / 2;
	constexpr static float m_appearY = 448;

	constexpr static float m_fireTime = 10.0f;
	constexpr static float m_deathbombTime = 15.0f;
	constexpr static float m_appearTime = 60.0f;

	constexpr static int m_powerMax = 128;

	using State = void(Player::*)(float);

	void mNormalState(float delta);
	void mDyingState(float delta);
	void mAppearingState(float delta);

	void mSetNormalState();
	void mSetDyingState();
	void mSetAppearingState();

	State m_state = nullptr;

	int m_score = 0;
	int m_lives = 5;
	int m_bombs = 3;
	int m_power = 0;
	int m_graze = 0;
	int m_point = 0;

	float m_fireTimer = 0.0f;
	float m_deathbombTimer = 0.0f;
	float m_appearTimer = 0.0f;
};
