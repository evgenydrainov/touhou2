#pragma once
#include <SFML/Graphics.hpp>

class PauseMenu
{
public:
	void update(float delta);
	void draw(sf::RenderTarget& target, float delta) const;

private:
	size_t m_cursor = 0;
	std::vector<std::string> m_menu =
	{
		"Resume",
		"Restart",
		"Option",
		"Quit"
	};
};
