#include "Boss.h"

#include "Game.h"

#include "mymath.h"
#include "collisions.h"
#include "Log.h"

using namespace luabridge;

Boss::Boss(luabridge::LuaRef bossData) :
	m_co(bossData.state())
{
	m_x = STGEngine::playAreaW / 2;
	m_y = STGEngine::playAreaH / 4;

	m_name = bossData["Name"].cast<std::string>();
	
	LuaRef phases = bossData["Phases"];
	size_t i = 0;
	LuaRef phase = phases[i];
	while (!phase.isNil())
	{
		m_phases.push_back({ phase["Hp"], phase["Time"], phase["Script"] });
		i++;
		phase = phases[i];
	}

	m_phaseInd = 0;
	m_startPhase();
}

void Boss::update(float delta)
{
	if (!m_co.isNil())
		LuaRef r = getGlobal(m_co.state(), "coroutine")["resume"](m_co, this);

	m_timer -= delta / 60.0f;
	if (m_timer <= 0.0f)
		m_endPhase();
}

void Boss::checkCollisions()
{
	auto& game = Game::getInstance();
	for (auto pb = game.engine->playerBullets.begin(); pb != game.engine->playerBullets.end(); )
		if (col::circle_vs_circle(m_x, m_y, m_radius, pb->getX(), pb->getY(), pb->getRadius()))
		{
			m_getDamage(10.0f);
			pb = game.engine->playerBullets.erase(pb);
		}
		else
		{
			++pb;
		}
}

void Boss::draw(sf::RenderTexture& target, float delta) const
{
	sf::RectangleShape r;
	r.setPosition(m_x, m_y);
	r.setSize(sf::Vector2f(24.0f, 48.0f));
	r.setOrigin(12.0f, 24.0f);
	target.draw(r);
}

float Boss::getMaxHp() const
{
	if (m_phaseInd >= 0 && m_phaseInd < m_phases.size())
		return m_phases[m_phaseInd].hp;

	return 0.0f;
}

void Boss::m_startPhase()
{
	if (m_phaseInd >= 0 && m_phaseInd < m_phases.size())
	{
		m_hp = m_phases[m_phaseInd].hp;
		m_timer = m_phases[m_phaseInd].time;
		m_co = getGlobal(m_co.state(), "coroutine")["create"](m_phases[m_phaseInd].script);
	}
}

void Boss::m_endPhase()
{
	if (m_phaseInd + 1 < m_phases.size())
	{
		m_phaseInd++;
		m_startPhase();
	}
	else
	{
		m_co = Nil();
		m_dead = true;
	}
}

void Boss::m_getDamage(float dmg)
{
	m_hp -= dmg;
	if (m_hp <= 0.0f)
		m_endPhase();
}
