#pragma once
#include "Singleton.h"

class Input : public Singleton<Input>
{
public:
	using State = size_t;

private:
	struct Codes
	{
		constexpr static State
			Up = 1,
			Down = 1 << 1,
			Left = 1 << 2,
			Right = 1 << 3,
			Z = 1 << 4,
			X = 1 << 5,
			Shift = 1 << 6,
			Enter = 1 << 7,
			Escape = 1 << 8;
	};

public:
	constexpr static State
		Up = Codes::Up,
		Down = Codes::Down,
		Left = Codes::Left,
		Right = Codes::Right,

		Confirm = Codes::Z | Codes::Enter,
		Cancel = Codes::X | Codes::Escape,
		Pause = Codes::Escape | Codes::Enter,

		Fire = Codes::Z,
		Bomb = Codes::X,
		Focus = Codes::Shift;

	void update();

	constexpr bool check(State key) const { return m_state & key; }
	constexpr bool checkPressed(State key) const { return m_statePress & key; }
	constexpr bool checkReleased(State key) const { return m_stateRelease & key; }

private:
	State m_state;
	State m_statePress;
	State m_stateRelease;
};
