#pragma once
#include <SFML/Graphics.hpp>

template <sf::Keyboard::Key K>
inline bool key_pressed()
{
	static bool was_pressed = sf::Keyboard::isKeyPressed(K);
	bool is_pressed = sf::Keyboard::isKeyPressed(K);

	bool result = false;
	if (is_pressed && !was_pressed)
		result = true;
	
	was_pressed = is_pressed;

	return result;
}
