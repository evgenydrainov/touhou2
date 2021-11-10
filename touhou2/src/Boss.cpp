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
		m_phases.emplace_back(phase["Hp"], phase["Time"], phase["Script"]);
		i++;
		phase = phases[i];
	}

	m_phaseInd = 0;
	mStartPhase();
}

void Boss::update(float delta)
{
	if (!m_co.isNil())
		LuaRef r = getGlobal(m_co.state(), "coroutine")["resume"](m_co, this);

	m_timer -= delta / 60.0f;
	if (m_timer <= 0.0f)
		mEndPhase();
}

void Boss::checkCollisions()
{
	auto& game = Game::getInstance();
	for (auto pb = game.engine->playerBullets.begin(); pb != game.engine->playerBullets.end(); )
		if (col::circle_vs_circle(m_x, m_y, m_radius, pb->getX(), pb->getY(), pb->getRadius()))
		{
			mGetDamage(10.0f);
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

void Boss::luaRegister(luabridge::Namespace nameSpace)
{
	nameSpace
		.beginClass<Boss>("_C_BOSS")
		.addProperty("x", &Boss::m_x)
		.addProperty("y", &Boss::m_y)
		.endClass();
}

float Boss::getMaxHp() const
{
	if (m_phaseInd >= 0 && m_phaseInd < m_phases.size())
		return m_phases[m_phaseInd].hp;

	return 0.0f;
}

void Boss::mStartPhase()
{
	if (m_phaseInd >= 0 && m_phaseInd < m_phases.size())
	{
		m_hp = m_phases[m_phaseInd].hp;
		m_timer = m_phases[m_phaseInd].time;
		m_co = getGlobal(m_co.state(), "coroutine")["create"](m_phases[m_phaseInd].script);
	}
}

void Boss::mEndPhase()
{
	if (m_phaseInd + 1 < m_phases.size())
	{
		m_phaseInd++;
		mStartPhase();
	}
	else
	{
		m_co = Nil();
		die();
		return;
	}
}

void Boss::mGetDamage(float dmg)
{
	m_hp -= dmg;
	if (m_hp <= 0.0f)
		mEndPhase();
}
