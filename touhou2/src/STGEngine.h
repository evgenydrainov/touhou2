#pragma once
#include "Player.h"
#include "Boss.h"
#include "Bullet.h"

#include "Script.h"

#include <SFML/Graphics.hpp>
#include <memory>

class STGEngine
{
public:
	constexpr static int playAreaW = 384;
	constexpr static int playAreaH = 448;
	constexpr static int playAreaX = 32;
	constexpr static int playAreaY = 16;

	enum class State
	{
		Null,
		Normal,
		Paused,
		GameOver
	};

	STGEngine();

	void update(float delta);
	void draw(sf::RenderTarget& target, float delta) const;

	sf::Texture projectiles;
	sf::Texture background;
	sf::Texture hud;

private:
	// this is here for destruction order
	Script m_script;

public:
	Player player;
	std::unique_ptr<Boss> boss;
	std::vector<Bullet> bullets;
	std::vector<Bullet> playerBullets;

private:
	constexpr static int m_hudX = 16;
	constexpr static int m_hudY = 32;
	
	void m_updateAll(float delta);
	void m_moveAll(float delta);
	void m_checkBoundsAll();

	void m_moveHighP(float physicsDelta);
	void m_moveLowP(float delta);
	void m_checkCollisionsHighP();
	void m_checkCollisionsLowP();

	void m_drawAll(float delta) const;
	void m_batchBullets(float delta) const;
	void m_debugDraw(float delta) const;

	void m_drawBg(sf::RenderTarget& target, float delta) const;
	void m_drawPlayArea(sf::RenderTarget& target, float delta) const;
	void m_drawHud(sf::RenderTarget& target, float delta) const;

	State m_state{};

	mutable sf::VertexArray m_bulletsBuf{ sf::Quads };
	mutable sf::RenderTexture m_playArea;
};
