#include "GameObject.h"

#include "mymath.h"

void GameObject::move(float delta)
{
	m_x += m_xspeed * delta;
	m_y += m_yspeed * delta;
}

void GameObject::setVelocity(float speed, float direction)
{
	m_xspeed = speed * math::cos(direction);
	m_yspeed = speed * -math::sin(direction);
}

float GameObject::getSpeed() const
{
	return math::point_direction(0.0f, 0.0f, m_xspeed, m_yspeed);
}

float GameObject::getDirection() const
{
	return math::point_direction(0.0f, 0.0f, m_xspeed, m_yspeed);
}
