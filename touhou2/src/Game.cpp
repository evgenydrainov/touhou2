#include "Game.h"

#include "Input.h"
#include "STGEngine.h"

#include "drawing.h"
#include "Text.h"

#include <fmt/format.h>

#include "Log.h"

void Game::run()
{
	m_loadAssets();
	m_initialize();

	sf::Clock c;
	while (m_window.isOpen())
	{
		m_skipFrame = false;

		m_handleEvents();

		if (!m_window.hasFocus())
			m_skipFrame = true;

		sf::Time t = c.restart();
		float delta = t.asSeconds() * 60.0f;

		if (!m_skipFrame)
			m_tick(delta);

		m_render();
	}
}

void Game::m_loadAssets()
{
	m_window.create(sf::VideoMode(gameW, gameH), "Touhou");
	m_window.setVerticalSyncEnabled(true);

	m_gameSurf.create(gameW, gameH);

	simsun.loadFromFile("Fonts/simsunb.ttf");

	auto& engine = STGEngine::getInstance();
	engine.loadAssets();
}

void Game::m_initialize()
{
	state = State::Gameplay;
	auto& engine = STGEngine::getInstance();
	engine.initialize();
}

void Game::m_handleEvents()
{
	sf::Event e;
	while (m_window.pollEvent(e))
		switch (e.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::PosChanging:
			m_skipFrame = true;
			break;
		}
}

void Game::m_tick(float delta)
{
	auto& input = Input::getInstance();
	auto& engine = STGEngine::getInstance();

	input.update();

	switch (state)
	{
	case State::Gameplay:
		engine.update(delta);
		break;
	}

	m_gameSurf.clear();

	switch (state)
	{
	case State::Gameplay:
		engine.draw(m_gameSurf, delta);
		break;
	}

	m_displayFpsCount += 60.0f / delta;
	m_displayFpsCountN += 1.0f;
	m_displayFpsTimer -= delta;

	if (m_displayFpsTimer <= 0.0f)
	{
		m_displayFps = m_displayFpsCount / m_displayFpsCountN;
		m_displayFpsCount = 0.0f;
		m_displayFpsCountN = 0.0f;
		m_displayFpsTimer = 60.0f;
	}

	Text t;
	t.setPosition(gameW, gameH);
	t.setString(fmt::format(
		"{:.2f}fps",
		m_displayFps));
	t.align(Text::HAlign::Right, Text::VAlign::Bottom);
	m_gameSurf.draw(t);

	m_gameSurf.display();
}

void Game::m_render()
{
	m_window.clear();
	m_window.draw(sf::Sprite(m_gameSurf.getTexture()));
	m_window.display();
}
