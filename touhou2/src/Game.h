#pragma once
#include "Singleton.h"

#include <SFML/Graphics.hpp>

class Game : public Singleton<Game>
{
public:
	constexpr static int gameW = 640;
	constexpr static int gameH = 480;

	enum class State
	{
		Null,
		Title,
		Gameplay,
		Closing
	};

	void run();

	State state{};

	sf::Font simsun;

private:
	void m_loadAssets();
	void m_initialize();

	void m_handleEvents();
	void m_tick(float delta);
	void m_render();

	float m_displayFps = 0.0f;
	float m_displayFpsTimer = 0.0f;
	float m_displayFpsCount = 0.0f;
	float m_displayFpsCountN = 0.0f;

	sf::RenderWindow m_window;
	sf::RenderTexture m_gameSurf;

	bool m_skipFrame = false;
};
