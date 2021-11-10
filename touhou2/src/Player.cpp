#include "Player.h"

#include "Input.h"
#include "Game.h"

#include "mymath.h"
#include "collisions.h"

Player::Player()
{
	m_radius = 2.0f;
	m_x = m_startX;
	m_y = m_startY;
	mSetNormalState();
}

void Player::update(float delta)
{
	if (m_state)
		(this->*m_state)(delta);
}

void Player::checkCollisions()
{
	if (m_state == &Player::mNormalState)
	{
		auto& game = Game::getInstance();
		for (auto b = game.engine->bullets.begin(); b != game.engine->bullets.end(); )
			if (col::circle_vs_circle(m_x, m_y, m_radius, b->getX(), b->getY(), b->getRadius()))
			{
				mSetDyingState();
				b = game.engine->bullets.erase(b);
				return;
			}
			else
			{
				++b;
			}
	}
}

void Player::checkBounds()
{
	if (m_state != &Player::mAppearingState)
	{
		m_x = std::clamp(m_x, 0.0f, (float)STGEngine::playAreaW);
		m_y = std::clamp(m_y, 0.0f, (float)STGEngine::playAreaH);
	}
}

void Player::animate(float delta)
{
}

void Player::draw(sf::RenderTexture& target, float delta) const
{
	sf::RectangleShape r;
	r.setPosition(m_x, m_y);
	r.setSize({ 24.0f, 48.0f });
	r.setOrigin(12.0f, 24.0f);
	target.draw(r);
}

void Player::luaRegister(luabridge::Namespace nameSpace)
{
	nameSpace
		.beginClass<Player>("_C_PLAYER")
		.addProperty("x", &Player::getX)
		.addProperty("y", &Player::getY)
		.endClass();
}

void Player::mNormalState(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	float s = input.check(Input::Focus) ? m_focusSpeed : m_moveSpeed;
	int h = input.check(Input::Right) - input.check(Input::Left);
	int v = input.check(Input::Down) - input.check(Input::Up);

	m_xspeed = h * s * (v != 0 ? math::sin(45.0f) : 1.0f);
	m_yspeed = v * s * (h != 0 ? math::sin(45.0f) : 1.0f);

	if (m_fireTimer <= 0.0f)
	{
		if (input.check(Input::Fire))
		{
			game.engine->playerBullets.emplace_back(m_x, m_y, 8.0f, 90.0f, 5.0f);
			m_fireTimer = m_fireTime;
		}
	}
	else
	{
		m_fireTimer -= delta;
	}

	if (input.checkPressed(Input::Bomb))
		if (m_bombs > 0)
		{
			game.engine->bullets.clear();
			m_bombs--;
		}
}

void Player::mDyingState(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	if (m_deathbombTimer <= 0.0f)
	{
		if (m_lives > 0)
		{
			m_lives--;
			mSetAppearingState();
		}
		else
		{
			die();
			return;
		}
	}
	else
	{
		m_deathbombTimer -= delta;
		if (input.checkPressed(Input::Bomb))
		{
			if (m_bombs > 0)
			{
				game.engine->bullets.clear();
				m_bombs--;
				mSetNormalState();
			}
		}
	}
}

void Player::mAppearingState(float delta)
{
	m_appearTimer += delta;
	m_x = math::lerp(m_appearX, m_startX, m_appearTimer / m_appearTime);
	m_y = math::lerp(m_appearY, m_startY, m_appearTimer / m_appearTime);
	if (m_appearTimer >= m_appearTime)
	{
		m_x = m_startX;
		m_y = m_startY;
		mSetNormalState();
	}
}

void Player::mSetNormalState()
{
	m_state = &Player::mNormalState;
	m_fireTimer = 0.0f;
}

void Player::mSetDyingState()
{
	m_state = &Player::mDyingState;
	m_deathbombTimer = m_deathbombTime;
	m_xspeed = 0.0f;
	m_yspeed = 0.0f;
}

void Player::mSetAppearingState()
{
	m_state = &Player::mAppearingState;
	m_appearTimer = 0.0f;
	m_x = m_appearX;
	m_y = m_appearY;
}
