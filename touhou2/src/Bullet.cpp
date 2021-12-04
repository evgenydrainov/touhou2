#include "Bullet.h"

#include "Game.h"

#include "mymath.h"
#include "Log.h"

using namespace luabridge;

Bullet::Bullet() :
	co(Game::getInstance().stage->script.L.get())
{
}

void Bullet::update(float delta)
{
	if (co_running)
	{
		co_timer += delta;
		while (co_timer > 0.0f)
		{
			setGlobal(co.state(), this, "self");
			co_running = getGlobal(co.state(), "coroutine")["resume"](co);

			if (!co_running)
			{
				co_timer = 0.0f;
				break;
			}

			co_timer--;
		}
	}
}

void Bullet::physicsUpdate(float delta)
{
	x += speed * math::dcos(direction) * delta;
	y += speed * -math::dsin(direction) * delta;
}

void Bullet::endUpdate(float delta)
{
	if (x < -100.0f || y < -100.0f || x >= Stage::playAreaW + 100.0f || y >= Stage::playAreaH + 100.0f)
	{
		dead = true;
		return;
	}
}

void Bullet::luaRegister(Namespace nameSpace)
{
	nameSpace
		.beginClass<Bullet>("_C_BULLET")
		.addProperty("x", &Bullet::x)
		.addProperty("y", &Bullet::y)
		.addProperty("speed", &Bullet::speed)
		.addProperty("direction", &Bullet::direction)
		.addProperty("radius", &Bullet::radius)
		.addProperty("rotate", &Bullet::rotate)
		.endClass();
}
