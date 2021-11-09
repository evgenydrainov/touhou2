#include "Text.h"

#include "Game.h"

Text::Text()
{
	// defaults
	auto& game = Game::getInstance();
	setFont(game.simsun);
	setCharacterSize(16);
}

void Text::align(HAlign halign, VAlign valign)
{
	auto b = getLocalBounds();
	sf::Vector2f o;
	
	switch (halign)
	{
	case HAlign::Center:
		o.x = b.left + 0.5f * b.width;
		break;

	case HAlign::Right:
		o.x = b.width + 2.0f * b.left;
		break;
	}

	switch (valign)
	{
	case VAlign::Middle:
		o.y = b.top + 0.5f * b.width;
		break;

	case VAlign::Bottom:
		o.y = b.height + 2.0f * b.top;
		break;
	}

	setOrigin(o);
}
