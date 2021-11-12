#include "Bullet.h"

#include "STGEngine.h"

#include "mymath.h"
#include "Log.h"

using namespace luabridge;

Bullet::Bullet(float x, float y, float speed, float direction, float radius) :
	x(x),
	y(y),
	speed(speed),
	direction(direction),
	radius(radius)
{
}

void Bullet::update(float delta)
{
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
