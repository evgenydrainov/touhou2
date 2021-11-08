#pragma once

class GameObject
{
public:
	void move(float delta);

	void setVelocity(float speed, float direction);

	float getSpeed() const;
	float getDirection() const;

	inline float getX() const { return m_x; }
	inline float getY() const { return m_y; }
	inline float getRadius() const { return m_radius; }
	inline bool isDead() const { return m_dead; }

protected:
	float m_x = 0.0f;
	float m_y = 0.0f;
	float m_xspeed = 0.0f;
	float m_yspeed = 0.0f;
	float m_radius = 0.0f;
	bool m_dead = false;
};
