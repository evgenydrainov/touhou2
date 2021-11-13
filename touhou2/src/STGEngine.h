#pragma once
#include "Player.h"
#include "Boss.h"
#include "Bullet.h"

#include "Script.h"

#include "PauseMenu.h"
#include "GameOverMenu.h"

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

	void resume();
	void useContinue();

	inline void toggle_hitboxes() { m_show_hitboxes ^= true; }

	sf::Texture projectiles;
	sf::Texture background;
	sf::Texture hud;

private:
	// this is here for destruction order
	Script m_script;

public:
	Player player{ m_script.getL() };
	std::unique_ptr<Boss> boss;
	std::vector<Bullet> bullets;
	std::vector<Bullet> playerBullets;

private:
	constexpr static int m_hudX = 16;
	constexpr static int m_hudY = 32;
	
	void mUpdateAll(float delta);
	void mPhysicsUpdateAll(float delta);
	void mEndUpdateAll(float delta);

	void mPhysicsUpdateHighP(float physicsDelta);
	void mPhysicsUpdateLowP(float delta);
	
	void mDrawAll(float delta) const;
	void mBatchBullets(float delta) const;
	void mDebugDraw(float delta) const;

	void mDrawBg(sf::RenderTarget& target, float delta) const;
	void mDrawPlayArea(sf::RenderTarget& target, float delta) const;
	void mDrawHud(sf::RenderTarget& target, float delta) const;

	State m_state{};

	mutable sf::VertexArray m_bulletsBuf{ sf::Quads };
	mutable sf::RenderTexture m_playArea;

	bool m_show_hitboxes = false;

	std::unique_ptr<PauseMenu> m_pauseMenu;
	std::unique_ptr<GameOverMenu> m_gameOverMenu;
};
