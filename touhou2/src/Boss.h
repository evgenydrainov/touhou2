#pragma once
#include "GameObject.h"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <SFML/Graphics.hpp>

class Boss : public GameObject
{
public:
	struct Phase
	{
		float hp, time;
		luabridge::LuaRef script;
	};

	Boss(luabridge::LuaRef bossData);

	void update(float delta);
	void checkCollisions();
	void draw(sf::RenderTexture& target, float delta) const;

	inline const std::string& getName() const { return m_name; }
	inline float getHp() const { return m_hp; }
	float getMaxHp() const;
	inline size_t getPhase() const { return m_phaseInd + 1; }
	inline size_t getPhasesAmount() const { return m_phases.size(); }
	inline float getTimer() const { return m_timer; }

private:
	void m_startPhase();
	void m_endPhase();
	void m_getDamage(float dmg);

	std::string m_name;
	float m_hp = 0.0f;
	std::vector<Phase> m_phases;
	size_t m_phaseInd = 0;
	float m_timer = 0.0f;

	luabridge::LuaRef m_co;
};
