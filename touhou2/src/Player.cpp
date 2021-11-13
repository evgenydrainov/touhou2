#include "Player.h"

#include "Input.h"
#include "Game.h"

#include "mymath.h"
#include "collisions.h"

using namespace luabridge;

Player::Player(lua_State* L) :
	L(L)
{
	radius = 2.0f;
	x = startX;
	y = startY;
	setNormalState();
}

void Player::update(float delta)
{
	if (state)
		(this->*state)(delta);
}

void Player::physicsUpdate(float physicsDelta)
{
	if (state == &Player::normalState)
	{
		x += speed * math::dcos(direction) * physicsDelta;
		y += speed * -math::dsin(direction) * physicsDelta;

		if (invincibility <= 0.0f)
		{
			auto& game = Game::getInstance();
			for (auto b = game.engine->bullets.begin(); b != game.engine->bullets.end(); )
				if (col::circle_vs_circle(x, y, radius, b->x, b->y, b->radius))
				{
					getHit();
					b = game.engine->bullets.erase(b);
					// don't get hit multiple times in 1 frame
					break;
				}
				else
				{
					++b;
				}
		}
	}
}

void Player::endUpdate(float delta)
{
	if (state != &Player::appearingState)
	{
		y = std::clamp(y, 0.0f, (float)STGEngine::playAreaH);
		x = std::clamp(x, 0.0f, (float)STGEngine::playAreaW);
	}

	// animation goes here
}

void Player::draw(sf::RenderTexture& target, float delta) const
{
	auto& game = Game::getInstance();

	if (invincibility > 0.0f)
		if (game.frame % 2 == 0)
			return;

	sf::RectangleShape r;
	r.setPosition(x, y);
	r.setSize({ 24.0f, 48.0f });
	r.setOrigin(12.0f, 24.0f);
	target.draw(r);
}

void Player::getHit()
{
	setDyingState();
	invincibility = 120.0f;
}

void Player::luaRegister(Namespace nameSpace)
{
	nameSpace
		.beginClass<Player>("_C_PLAYER")
		.addProperty("x", &Player::x)
		.addProperty("y", &Player::y)
		.addProperty("speed", &Player::speed)
		.addProperty("direction", &Player::direction)
		.addProperty("radius", &Player::radius)
		.endClass();
}

void Player::normalState(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	if (invincibility > 0.0f)
		invincibility -= delta;

	focus = input.check(Input::Focus);

	int h = input.check(Input::Right) - input.check(Input::Left);
	int v = input.check(Input::Down) - input.check(Input::Up);
	if (h != 0 || v != 0)
	{
		speed = focus ? focusSpeed : moveSpeed;
		direction = math::point_direction(0.0f, 0.0f, h, v);
	}
	else
	{
		speed = 0.0f;
	}

	if (fireTimer <= 0.0f)
	{
		if (input.check(Input::Fire))
		{
			game.engine->playerBullets.emplace_back(x, y, 8.0f, 90.0f, 5.0f, L);
			fireTimer = fireTime;
		}
	}
	else
	{
		fireTimer -= delta;
	}

	if (input.checkPressed(Input::Bomb))
		if (bombs > 0)
		{
			game.engine->bullets.clear();
			bombs--;
		}
}

void Player::dyingState(float delta)
{
	auto& input = Input::getInstance();
	auto& game = Game::getInstance();

	if (deathbombTimer <= 0.0f)
	{
		if (lives > 0)
		{
			lives--;
			setAppearingState();
		}
		else
		{
			dead = true;
			return;
		}
	}
	else
	{
		deathbombTimer -= delta;
		if (input.checkPressed(Input::Bomb))
		{
			if (bombs > 0)
			{
				game.engine->bullets.clear();
				bombs--;
				setNormalState();
			}
		}
	}
}

void Player::appearingState(float delta)
{
	appearTimer += delta;
	x = math::lerp(appearX, startX, appearTimer / appearTime);
	y = math::lerp(appearY, startY, appearTimer / appearTime);
	if (appearTimer >= appearTime)
	{
		x = startX;
		y = startY;
		setNormalState();
	}
}

void Player::setNormalState()
{
	state = &Player::normalState;
	fireTimer = 0.0f;
}

void Player::setDyingState()
{
	state = &Player::dyingState;
	deathbombTimer = deathbombTime;
}

void Player::setAppearingState()
{
	state = &Player::appearingState;
	appearTimer = 0.0f;
	x = appearX;
	y = appearY;
}
