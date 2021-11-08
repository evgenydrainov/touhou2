#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Bullet : public GameObject
{
public:
	Bullet(float x, float y, float speed, float direction, float radius);

	void checkBounds();
};
