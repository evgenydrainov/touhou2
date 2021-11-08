#pragma once
#include <SFML/Graphics.hpp>

namespace draw
{
	inline void circle(sf::RenderTexture& target, float x, float y, float r, const sf::Color& col = sf::Color::White)
	{
		sf::CircleShape c;
		c.setPosition(x, y);
		c.setRadius(r);
		c.setOrigin(r, r);
		c.setFillColor(col);
		target.draw(c);
	}
}
