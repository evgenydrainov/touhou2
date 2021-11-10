#include "Bullet.h"

#include "STGEngine.h"

#include "mymath.h"

Bullet::Bullet(float x, float y, float speed, float direction, float radius)
{
	m_x = x;
	m_y = y;
	m_radius = radius;
	setVelocity(speed, direction);
}

void Bullet::checkBounds()
{
	if (m_x < 0.0f || m_y < 0.0f || m_x >= STGEngine::playAreaW || m_y >= STGEngine::playAreaH)
	{
		die();
		return;
	}
}
