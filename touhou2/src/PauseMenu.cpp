#include "PauseMenu.h"

#include "Input.h"
#include "Game.h"

#include "Text.h"

void PauseMenu::update(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	if (input.checkPressed(Input::Confirm))
	{
		switch (m_cursor)
		{
		case 0:
			game.engine->resume();
			return;

		case 1:
			game.engine = nullptr;
			game.engine = std::make_unique<STGEngine>();
			break;

		case 2:

			break;

		case 3:

			break;
		}
	}
	else if (input.checkPressed(Input::Pause))
	{
		game.engine->resume();
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

void PauseMenu::draw(sf::RenderTarget& target, float delta) const
{
	sf::RectangleShape r;
	r.setSize({ STGEngine::playAreaW, STGEngine::playAreaH });
	r.setFillColor({ 0, 0, 0, 128 });
	target.draw(r);

	for (size_t i = 0; i < m_menu.size(); i++)
	{
		Text t;
		t.setPosition(STGEngine::playAreaW / 2, STGEngine::playAreaH / 3 + 16 * i);
		if (i == m_cursor)
			t.setFillColor(sf::Color::Yellow);

		t.setString(m_menu[i]);
		t.align(Text::HAlign::Center, Text::VAlign::Top);
		target.draw(t);
	}
}
