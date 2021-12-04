#pragma once
#include <SFML/Graphics.hpp>

class PauseOptionsMenu
{
public:
	PauseOptionsMenu(class PauseMenu& owner);

	void update(float delta);
	void draw(sf::RenderTarget& target, float delta) const;

private:
	class PauseMenu& m_owner;

	size_t m_cursor = 0;
	std::vector<std::string> m_menu;
};
