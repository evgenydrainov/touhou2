#pragma once
#include "Singleton.h"

#include "STGEngine.h"

#include <SFML/Graphics.hpp>
#include <memory>

class Game : public Singleton<Game>
{
public:
	constexpr static int gameW = 640;
	constexpr static int gameH = 480;

	void run();

	std::unique_ptr<STGEngine> engine;
	//std::unique_ptr<TitleScreen> title;

	sf::Font simsun;

private:
	enum class State
	{
		Null,
		Title,
		Gameplay,
	};

	enum class Fullscreen
	{
		Disabled,
		Enabled,
		Borderless
	};

	void mHandleEvents();
	void mTick(float delta);
	void mRender() const;

	void mUpdate(float delta);
	void mDraw(sf::RenderTarget& target, float delta) const;

	State m_state{};
	
	mutable sf::RenderWindow m_window;
	mutable sf::RenderTexture m_gameSurf;

	bool m_skipFrame = false;

	float m_displayFps = 0.0f;
	float m_displayFpsTimer = 0.0f;
	float m_displayFpsCount = 0.0f;
	float m_displayFpsCountN = 0.0f;

	Fullscreen m_fullscreen = Fullscreen::Disabled;
};
