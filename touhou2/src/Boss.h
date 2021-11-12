#pragma once
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SFML/Graphics.hpp>

struct Boss
{
public:
	// boss definitions
	struct Phase
	{
		float hp, time;
		luabridge::LuaRef script;

		Phase(float hp, float time, luabridge::LuaRef script) :
			hp(hp), time(time), script(script)
		{
		}
	};

public:
	// standard methods
	Boss(luabridge::LuaRef bossData);

	void update(float delta);
	void physicsUpdate(float delta);
	void endUpdate(float delta);
	void draw(sf::RenderTexture& target, float delta) const;

public:
	// standard fields
	bool dead = false;
	float x = 0.0f;
	float y = 0.0f;
	float speed = 0.0f;
	float direction = 0.0f;
	float acc = 0.0f;
	float speedMin = 0.0f;
	float radius = 0.0f;

public:
	// boss methods
	float getMaxHp() const;
	size_t getPhasesLeft() const;

	void startPhase();
	void endPhase();
	void getDamage(float dmg);

public:
	// boss fields
	float hp = 0.0f;
	float timer = 0.0f;
	size_t phaseInd = 0;
	std::string name;
	std::vector<Phase> phases;

	luabridge::LuaRef co;
	float co_timer = 0.0f;
	bool co_finished = false;

public:
	// other
	static void luaRegister(luabridge::Namespace nameSpace);
};
