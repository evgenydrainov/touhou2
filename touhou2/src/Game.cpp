#include "Game.h"

#include "Input.h"

#include "mymath.h"
#include "Text.h"
#include "key_pressed.h"
#include <fmt/format.h>
#include "Log.h"
#include <inipp/inipp.h>
#include <fstream>

void Game::run()
{
	loadOptions();
	m_gameSurf.create(gameW, gameH);
	simsun.loadFromFile("Fonts/simsunb.ttf");
	startStage();

	sf::Clock sf_clock;
	while (m_window.isOpen())
	{
		sf::Event e;
		while (m_window.pollEvent(e))
			if (e.type == sf::Event::Closed)
				m_window.close();

		float delta = std::clamp(sf_clock.restart().asSeconds(), std::numeric_limits<float>::min(), 1.0f / 30.0f) * 60.0f;

		if (m_window.hasFocus())
			mTick(delta);

		mRender();
	}
}

void Game::goToTitleScren()
{
	if (stage)
	{
		delete stage;
		stage = nullptr;
	}

	//if (title)
	//{
	//	delete title;
	//	title = nullptr;
	//}

	m_state = State::Title;
	//title = new TitleScreen;
}

void Game::startStage()
{
	if (stage)
	{
		delete stage;
		stage = nullptr;
	}

	//if (title)
	//{
	//	delete title;
	//	title = nullptr;
	//}

	m_state = State::Gameplay;
	stage = new Stage;
}

void Game::restartStage()
{
	startStage();
}

void Game::quitFromStage()
{
	m_window.close();
}

void Game::loadOptions()
{
	std::ifstream file("options.ini");
	inipp::Ini<char> ini;
	ini.parse(file);

	bool recreateFile = false;

	if (ini.sections["Video"]["Fullscreen"] == "Disabled")
		setFullscreen(Fullscreen::Disabled);
	else if (ini.sections["Video"]["Fullscreen"] == "Enabled")
		setFullscreen(Fullscreen::Enabled);
	else if (ini.sections["Video"]["Fullscreen"] == "Borderless")
		setFullscreen(Fullscreen::Borderless);
	else
	{
		setFullscreen(Fullscreen::Borderless);
		recreateFile = true;
	}

	if (ini.sections["Video"]["VSync"] == "Disabled")
		setVSync(false);
	else if (ini.sections["Video"]["VSync"] == "Enabled")
		setVSync(true);
	else
	{
		setVSync(true);
		recreateFile = true;
	}

	if (recreateFile)
		saveOptions();
}

void Game::saveOptions()
{
	inipp::Ini<char> ini;

	switch (m_fullscreen)
	{
	default:
	case Fullscreen::Disabled:
		ini.sections["Video"]["Fullscreen"] = "Disabled";
		break;

	case Fullscreen::Enabled:
		ini.sections["Video"]["Fullscreen"] = "Enabled";
		break;

	case Fullscreen::Borderless:
		ini.sections["Video"]["Fullscreen"] = "Borderless";
		break;
	}

	ini.sections["Video"]["VSync"] = m_vSync ? "Enabled" : "Disabled";

	std::ofstream file("options.ini");
	ini.generate(file);
}

void Game::setFullscreen(Fullscreen fullscreen)
{
	m_fullscreen = fullscreen;

	switch (m_fullscreen)
	{
	default:
	case Fullscreen::Disabled:
		m_window.create(sf::VideoMode(gameW, gameH), "Touhou");
		break;

	case Fullscreen::Enabled:
		m_window.create(sf::VideoMode::getFullscreenModes()[0], "Touhou", sf::Style::Fullscreen);
		break;

	case Fullscreen::Borderless:
		m_window.create(sf::VideoMode::getFullscreenModes()[0], "Touhou", sf::Style::None);
		break;
	}

	setVSync(m_vSync);
}

void Game::setVSync(bool enabled)
{
	m_vSync = enabled;
	m_window.setVerticalSyncEnabled(m_vSync);
}

void Game::mTick(float delta)
{
	auto& input = Input::getInstance();

	input.update();

	// debug
	show_hitboxes ^= key_pressed<sf::Keyboard::F3>();
	if (key_pressed<sf::Keyboard::F2>())
	{
		startStage();
	}
	else if (key_pressed<sf::Keyboard::F8>())
	{
		stage->boss->endPhase();
	}
	else if (key_pressed<sf::Keyboard::F9>())
	{
		setVSync(!getVSync());
		saveOptions();
	}

	switch (m_state)
	{
	case State::Title:
		//title-update(delta);
		break;

	case State::Gameplay:
		stage->update(delta);
		break;
	}

	m_gameSurf.clear();

	switch (m_state)
	{
	case State::Title:
		//title->draw(delta);
		break;

	case State::Gameplay:
		stage->draw(m_gameSurf, delta);
		break;
	}

	m_displayFpsCount += 60.0f / delta;
	m_displayFpsCountN += 1.0f;

	if (m_displayFpsCountN >= 60.0f)
	{
		m_displayFps = m_displayFpsCount / m_displayFpsCountN;
		m_displayFpsCount = 0.0f;
		m_displayFpsCountN = 0.0f;
	}

	Text t;
	t.setPosition(sf::Vector2f(m_gameSurf.getSize()));
	t.setString(fmt::format(
		"{:.2f}fps",
		m_displayFps));
	t.align(Text::HAlign::Right, Text::VAlign::Bottom);
	m_gameSurf.draw(t);

	m_gameSurf.display();
}

void Game::mRender() const
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
