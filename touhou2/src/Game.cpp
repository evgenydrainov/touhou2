#include "Game.h"

#include "Input.h"

#include "Text.h"
#include "key_pressed.h"
#include <fmt/format.h>
#include "Log.h"

void Game::run()
{
	m_window.create(sf::VideoMode(gameW, gameH), "Touhou");
	m_window.setVerticalSyncEnabled(true);
	m_gameSurf.create(gameW, gameH);
	simsun.loadFromFile("Fonts/simsunb.ttf");

	state = State::Gameplay;
	engine = std::make_unique<STGEngine>();

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

	input.update();

	// debug
	if (key_pressed<sf::Keyboard::F2>())
	{
		// restart
		state = State::Gameplay;
		engine = nullptr;
		engine = std::make_unique<STGEngine>();
	}

	//if (key_pressed<sf::Keyboard::F4>())
	//{
	//	m_window.create(sf::VideoMode::getFullscreenModes()[0], "Touhou", sf::Style::None);
	//	m_window.setVerticalSyncEnabled(true);
	//}

	m_update(delta);

	m_gameSurf.clear();

	m_draw(m_gameSurf, delta);

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
	t.setPosition(sf::Vector2f(m_gameSurf.getSize()));
	t.setString(fmt::format(
		"{:.2f}fps",
		m_displayFps));
	t.align(Text::HAlign::Right, Text::VAlign::Bottom);
	m_gameSurf.draw(t);

	static bool show_msg = false;
	show_msg ^= key_pressed<sf::Keyboard::F1>();
	if (show_msg)
	{
		Text t;
		t.setPosition(0.0f, m_gameSurf.getSize().y);
		t.setString("f1 - show this message\n"
			"f2 - restart");
		t.align(Text::HAlign::Left, Text::VAlign::Bottom);
		m_gameSurf.draw(t);
	}

	m_gameSurf.display();
}

void Game::m_render() const
{
	m_window.clear();

	sf::Vector2f windowSize(m_window.getSize());
	sf::Vector2f gameSize(m_gameSurf.getSize());
	float scale = std::min(windowSize.x / gameSize.x, windowSize.y / gameSize.y);

	sf::Sprite gameSurf(m_gameSurf.getTexture());
	gameSurf.setScale(scale, scale);
	gameSurf.setOrigin(gameSize * 0.5f);
	gameSurf.setPosition(windowSize * 0.5f);

	m_window.setView({ windowSize * 0.5f, windowSize });
	m_window.draw(gameSurf);

	m_window.display();
}

void Game::m_update(float delta)
{
	switch (state)
	{
	case State::Gameplay:
		engine->update(delta);
		break;
	}
}

void Game::m_draw(sf::RenderTarget& target, float delta) const
{
	switch (state)
	{
	case State::Gameplay:
		engine->draw(target, delta);
		break;
	}
}
