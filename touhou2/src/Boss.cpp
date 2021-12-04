#include "Boss.h"

#include "Game.h"

#include "mymath.h"
#include "collisions.h"
#include "Log.h"

using namespace luabridge;

Boss::Boss(LuaRef bossData) :
	//co(bossData.state())
	L(bossData.state())
{
	x = Stage::playAreaW / 2;
	y = Stage::playAreaH / 4;

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

	radius = 30.0f;
}

void Boss::update(float delta)
{
	timer -= delta / 60.0f;
	if (timer <= 0.0f)
		endPhase();

	if (!co.empty())
	{
		co_timer += delta;
		while (co_timer > 0.0f)
		{
			//for (auto it = co.begin(); it != co.end(); )
			//{
			//	setGlobal(it->state(), this, "self");
			//	if (getGlobal(it->state(), "coroutine")["resume"](*it).cast<bool>() == false)
			//		it = co.erase(it);
			//	else
			//		++it;
			//}

			// stuff can be added to co inside coroutine.resume
			for (size_t i = 0, n = co.size(); i < n; i++)
			{
				//Log(n);
				setGlobal(co[i].state(), this, "self");
				if (getGlobal(co[i].state(), "coroutine")["resume"](co[i]).cast<bool>() == false)
					co.erase(std::next(co.begin(), i));
			}
			
			if (co.empty())
			{
				co_timer = 0.0f;
				break;
			}

			co_timer--;
		}
	}
}

void Boss::physicsUpdate(float delta)
{
	x += speed * math::dcos(direction) * delta;
	y += speed * -math::dsin(direction) * delta;

	auto& game = Game::getInstance();
	for (auto pb = game.stage->playerBullets.begin(); pb != game.stage->playerBullets.end(); )
		if (col::circle_vs_circle(x, y, radius, pb->x, pb->y, pb->radius))
		{
			getDamage(10.0f);
			pb = game.stage->playerBullets.erase(pb);
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

void Boss::add_thread(luabridge::LuaRef f)
{
	// L doesn't work for some reason
	co.emplace_back(getGlobal(f.state(), "coroutine")["create"](f));
}

void Boss::luaRegister(Namespace nameSpace)
{
	nameSpace
		.beginClass<Boss>("_C_BOSS")
		.addProperty("x", &Boss::x)
		.addProperty("y", &Boss::y)
		.addProperty("speed", &Boss::speed)
		.addProperty("direction", &Boss::direction)
		.addProperty("radius", &Boss::radius)
		.addProperty("time_manipulator", &Boss::time_manipulator)
		.addFunction("add_thread", &Boss::add_thread)

		.addProperty("u", &Boss::u)
		.addProperty("v", &Boss::v)
		.addProperty("w", &Boss::w)
		.addProperty("h", &Boss::h)

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
		//co = getGlobal(co.state(), "coroutine")["create"](phases[phaseInd].script);
		//co_running = true;
		co.emplace_back(getGlobal(L, "coroutine")["create"](phases[phaseInd].script));
	}
}

void Boss::endPhase()
{
	auto& game = Game::getInstance();
	co.clear();
	game.stage->gameplayDelta = 1.0f;
	game.stage->bullets.clear();

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
