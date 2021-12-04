#include "GameOverMenu.h"

#include "Input.h"
#include "Game.h"

#include "Text.h"

void GameOverMenu::update(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	if (input.checkPressed(Input::Confirm))
	{
		switch (m_cursor)
		{
		case 0:
			// continue
			game.stage->useContinue();
			return;

		case 1:
			// quit
			break;
		}
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

void GameOverMenu::draw(sf::RenderTarget& target, float delta) const
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
