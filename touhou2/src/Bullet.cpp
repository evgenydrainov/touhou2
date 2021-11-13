#include "Bullet.h"

#include "STGEngine.h"

#include "mymath.h"
#include "Log.h"

using namespace luabridge;

Bullet::Bullet(float x, float y, float speed, float direction, float radius, lua_State* L) :
	x(x),
	y(y),
	speed(speed),
	direction(direction),
	radius(radius),
	co(L)
{
}

Bullet::Bullet(float x, float y, float speed, float direction, float radius, luabridge::LuaRef script, lua_State* L) :
	x(x),
	y(y),
	speed(speed),
	direction(direction),
	radius(radius),
	co(L)
{
	co = getGlobal(L, "coroutine")["create"](script);
	co_running = true;
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

	speed += acc * delta;
	speed = std::max(speed, speedMin);
}

void Bullet::physicsUpdate(float delta)
{
	x += speed * math::dcos(direction) * delta;
	y += speed * -math::dsin(direction) * delta;
}

void Bullet::endUpdate(float delta)
{
	if (x < 0.0f || y < 0.0f || x >= STGEngine::playAreaW || y >= STGEngine::playAreaH)
	{
		dead = true;
		//co = Nil();
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
		.endClass();
}
