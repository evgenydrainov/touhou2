#pragma once
#include <SFML/Graphics.hpp>

class Text : public sf::Text
{
public:
	enum class HAlign { Left, Center, Right };
	enum class VAlign { Top, Middle, Bottom };

	Text();

	// this modifies the origin
	void align(HAlign halign, VAlign valign);
};
