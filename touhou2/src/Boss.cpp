#include "Boss.h"

#include "Game.h"

#include "mymath.h"
#include "collisions.h"
#include "Log.h"

using namespace luabridge;

Boss::Boss(LuaRef bossData) :
	co(bossData.state())
{
	x = STGEngine::playAreaW / 2;
	y = STGEngine::playAreaH / 4;

	name = bossData["Name"].cast<std::string>();
	
	LuaRef _phases = bossData["Phases"];
	size_t _i = 1;
	LuaRef _phase = _phases[_i];
	while (!_phase.isNil())
	{
		phases.emplace_back(_phase["Hp"], _phase["Time"], _phase["Script"]);
		_i++;
		_phase = _phases[_i];
	}

	phaseInd = 0;
	startPhase();
}

void Boss::update(float delta)
{
	timer -= delta / 60.0f;
	if (timer <= 0.0f)
		endPhase();

	co_timer += delta;
	while (co_timer > 0.0f)
	{
		if (!co_finished)
			if (!co.isNil())
			{
				LuaRef r = getGlobal(co.state(), "coroutine")["resume"](co, this);
				if (!r.cast<bool>())
					co_finished = true;
			}

		co_timer--;
	}

	speed += acc * delta;
	speed = std::max(speed, speedMin);
}

void Boss::physicsUpdate(float delta)
{
	x += speed * math::dcos(direction) * delta;
	y += speed * -math::dsin(direction) * delta;

	auto& game = Game::getInstance();
	for (auto pb = game.engine->playerBullets.begin(); pb != game.engine->playerBullets.end(); )
		if (col::circle_vs_circle(x, y, radius, pb->x, pb->y, pb->radius))
		{
			getDamage(10.0f);
			pb = game.engine->playerBullets.erase(pb);
			// don't break, allow to get hit multiple times in 1 frame
		}
		else
		{
			++pb;
		}
}

void Boss::endUpdate(float delta)
{
	// animation
}

void Boss::draw(sf::RenderTexture& target, float delta) const
{
	sf::RectangleShape r;
	r.setPosition(x, y);
	r.setSize(sf::Vector2f(24.0f, 48.0f));
	r.setOrigin(12.0f, 24.0f);
	target.draw(r);
}

void Boss::luaRegister(Namespace nameSpace)
{
	nameSpace
		.beginClass<Boss>("_C_BOSS")
		.addProperty("x", &Boss::x)
		.addProperty("y", &Boss::y)
		.addProperty("speed", &Boss::speed)
		.addProperty("direction", &Boss::direction)
		.addProperty("acc", &Boss::acc)
		.addProperty("radius", &Boss::radius)
		.endClass();
}

float Boss::getMaxHp() const
{
	if (phaseInd >= 0 && phaseInd < phases.size())
		return phases[phaseInd].hp;

	return -1.0f;
}

size_t Boss::getPhasesLeft() const
{
	return phases.size() - phaseInd;
}

void Boss::startPhase()
{
	if (phaseInd >= 0 && phaseInd < phases.size())
	{
		hp = phases[phaseInd].hp;
		timer = phases[phaseInd].time;
		co = getGlobal(co.state(), "coroutine")["create"](phases[phaseInd].script);
	}
}

void Boss::endPhase()
{
	if (phaseInd + 1 < phases.size())
	{
		phaseInd++;
		startPhase();
	}
	else
	{
		dead = true;
		//co = Nil();
		return;
	}
}

void Boss::getDamage(float dmg)
{
	hp -= dmg;
	if (hp <= 0.0f)
		endPhase();
}
