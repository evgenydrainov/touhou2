#pragma once
#include "Player.h"
#include "Boss.h"
#include "Bullet.h"

#include "Script.h"

#include "PauseMenu.h"
#include "GameOverMenu.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <random>

class Stage
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

	Stage();

	void update(float delta);
	void draw(sf::RenderTarget& target, float delta) const;

	void pause();
	void resume();
	void useContinue();
	void showGameOverMenu();

	sf::Texture projectiles;
	sf::Texture background;
	sf::Texture hud;
	sf::Texture characters;

	// keep below objects
	Script script;

	Player player;
	std::unique_ptr<Boss> boss;
	std::vector<Bullet> bullets;
	std::vector<Bullet> playerBullets;

	float gameplayDelta = 1.0f;

	std::mt19937 randomEngine{ 69420 };
	inline float random(float from, float to)
	{
		float t = static_cast<float>(randomEngine()) / static_cast<float>(std::mt19937::max());
		return from + (to - from) * t;
	}

private:
	constexpr static int m_hudX = 16;
	constexpr static int m_hudY = 32;
	
	void mUpdateAll(float delta);
	void mPhysicsUpdateAll(float delta);
	void mEndUpdateAll(float delta);
	
	void mDrawAll(float delta) const;
	void mBatchBullets(float delta) const;
	void mDebugDraw(float delta) const;
	void mDrawPlayAreaHud(float delta) const;

	void mDrawBg(sf::RenderTarget& target, float delta) const;
	void mDrawPlayArea(sf::RenderTarget& target, float delta) const;
	void mDrawHud(sf::RenderTarget& target, float delta) const;

	State m_state = State::Normal;

	mutable sf::VertexArray m_bulletsBuf{ sf::Quads };
	mutable sf::RenderTexture m_playArea;

	PauseMenu* m_pauseMenu = nullptr;
	GameOverMenu* m_gameOverMenu = nullptr;

	sf::Texture m_scriptTexture;
};
