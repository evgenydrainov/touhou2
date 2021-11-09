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

	void m_handleEvents();
	void m_tick(float delta);
	void m_render() const;

	void m_update(float delta);
	void m_draw(sf::RenderTarget& target, float delta) const;

	State state{};

	mutable sf::RenderWindow m_window;
	mutable sf::RenderTexture m_gameSurf;

	bool m_skipFrame = false;

	float m_displayFps = 0.0f;
	float m_displayFpsTimer = 0.0f;
	float m_displayFpsCount = 0.0f;
	float m_displayFpsCountN = 0.0f;

	Fullscreen m_fullscreen = Fullscreen::Disabled;
};
