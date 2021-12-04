#include "PauseOptionsMenu.h"

#include "Input.h"
#include "Game.h"

#include "PauseMenu.h"

#include "Text.h"

PauseOptionsMenu::PauseOptionsMenu(PauseMenu& owner) :
	m_owner(owner)
{
}

void PauseOptionsMenu::update(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();
	
	if (m_menu.empty())
	{
		switch (game.getFullscreen())
		{
		case Game::Fullscreen::Disabled:
			m_menu.emplace_back("Fullscreen: Disabled >");
			break;

		case Game::Fullscreen::Enabled:
			m_menu.emplace_back("Fullscreen: < Enabled >");
			break;

		case Game::Fullscreen::Borderless:
			m_menu.emplace_back("Fullscreen: < Borderless");
			break;
		}

		if (game.getVSync())
			m_menu.emplace_back("VSync: < Enabled");
		else
			m_menu.emplace_back("VSync: Disabled >");

		m_menu.emplace_back("Back");
	}

	if (m_cursor == 0)
		if (input.checkPressed(Input::Right))
		{
			if (game.getFullscreen() == Game::Fullscreen::Disabled)
			{
				game.setFullscreen(Game::Fullscreen::Enabled);
				m_menu.clear();
			}
			else if (game.getFullscreen() == Game::Fullscreen::Enabled)
			{
				game.setFullscreen(Game::Fullscreen::Borderless);
				m_menu.clear();
			}
		}
		else if (input.checkPressed(Input::Left))
		{
			if (game.getFullscreen() == Game::Fullscreen::Enabled)
			{
				game.setFullscreen(Game::Fullscreen::Disabled);
				m_menu.clear();
			}
			else if (game.getFullscreen() == Game::Fullscreen::Borderless)
			{
				game.setFullscreen(Game::Fullscreen::Enabled);
				m_menu.clear();
			}
		}

	if (m_cursor == 1)
		if (input.checkPressed(Input::Left))
		{
			if (game.getVSync())
			{
				game.setVSync(false);
				m_menu.clear();
			}
		}
		else if (input.checkPressed(Input::Right))
		{
			if (!game.getVSync())
			{
				game.setVSync(true);
				m_menu.clear();
			}
		}

	if (input.checkPressed(Input::Confirm))
	{
		switch (m_cursor)
		{
		case 1:
			game.setVSync(!game.getVSync());
			m_menu.clear();
			break;

		case 2:
			m_owner.back();
			break;
		}
	}
	else if (input.checkPressed(Input::Pause | Input::Cancel))
	{
		m_owner.back();
		return;
	}
	else if (input.checkPressed(Input::Down))
	{
		m_cursor++;
		m_cursor %= m_menu.size();
	}
	else if (input.checkPressed(Input::Up))
	{
		if (m_cursor == 0)
			m_cursor = m_menu.size() - 1;
		else
			m_cursor--;
	}
}

void PauseOptionsMenu::draw(sf::RenderTarget& target, float delta) const
{
	sf::RectangleShape r;
	r.setSize({ Stage::playAreaW, Stage::playAreaH });
	r.setFillColor({ 0, 0, 0, 128 });
	target.draw(r);

	for (size_t i = 0; i < m_menu.size(); i++)
	{
		Text t;
		t.setPosition(Stage::playAreaW / 2, Stage::playAreaH / 3 + 16 * i);
		if (i == m_cursor)
			t.setFillColor(sf::Color::Yellow);

		t.setString(m_menu[i]);
		t.align(Text::HAlign::Center, Text::VAlign::Top);
		target.draw(t);
	}
}
