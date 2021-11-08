#include "Input.h"
#include <SFML/Graphics.hpp>

void Input::update()
{
	constexpr sf::Keyboard::Key keyboardCodes[] =
	{
		sf::Keyboard::Up,
		sf::Keyboard::Down,
		sf::Keyboard::Left,
		sf::Keyboard::Right,
		sf::Keyboard::Z,
		sf::Keyboard::X,
		sf::Keyboard::LShift,
		sf::Keyboard::Enter,
		sf::Keyboard::Escape,
	};

	State prevState = m_state;
	m_state = 0;

	for (size_t i = 0; i < 9; i++)
		m_state |= sf::Keyboard::isKeyPressed(keyboardCodes[i]) << i;

	m_statePress = m_state & prevState ^ m_state;
	m_stateRelease = m_state & prevState ^ prevState;
}
