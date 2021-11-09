#include "Script.h"

#include "Game.h"

#include "mymath.h"
#include "Log.h"

using namespace luabridge;

void CreateBullet(float x, float y, float speed, float direction, float radius)
{
	auto& game = Game::getInstance();
	game.engine->bullets.emplace_back(x, y, speed, direction, radius);
}

void CreateBoss(LuaRef bossData)
{
	auto& game = Game::getInstance();
	game.engine->boss = std::make_unique<Boss>(bossData);
}

float BossGetX(Boss* b)
{
	return b->getX();
}

float BossGetY(Boss* b)
{
	return b->getY();
}

float PlayerGetX(int playerInd)
{
	auto& game = Game::getInstance();
	return game.engine->player.getX();
}

float PlayerGetY(int playerInd)
{
	auto& game = Game::getInstance();
	return game.engine->player.getY();
}

Script::Script() :
	m_L(luaL_newstate(), lua_close),
	m_co(m_L.get())
{
	luaL_requiref(m_L.get(), "_G", luaopen_base, 1);
	luaL_requiref(m_L.get(), LUA_COLIBNAME, luaopen_coroutine, 1);
	lua_pop(m_L.get(), 2);
	
	getGlobalNamespace(m_L.get())
		.addFunction("CreateBullet", CreateBullet)
		.addFunction("CreateBoss", CreateBoss)
		.addFunction("BossGetX", BossGetX)
		.addFunction("BossGetY", BossGetY)
		.addFunction("PlayerGetX", PlayerGetX)
		.addFunction("PlayerGetY", PlayerGetY)

		.addFunction("point_direction", math::point_direction)
		.addFunction("point_distance", math::point_distance)
		.addFunction("random", math::random)
		
		.beginClass<Boss>("_C_BOSS")
		.endClass();
}

void Script::load(const std::string& fname)
{
	if (luaL_dofile(m_L.get(), fname.c_str()) == LUA_OK)
	{
		LuaRef stage = getGlobal(m_L.get(), "Stage");
		m_co = getGlobal(m_L.get(), "coroutine")["create"](stage["Script"]);
	}
	else
	{
		Log(lua_tostring(m_L.get(), -1));
	}
}

void Script::update(float delta)
{
	auto& game = Game::getInstance();
	setGlobal(m_L.get(), delta, "Delta");
	setGlobal(m_L.get(), game.engine->player.getX(), "PlayerX");
	setGlobal(m_L.get(), game.engine->player.getY(), "PlayerY");
	
	if (!m_co.isNil())
		LuaRef r = getGlobal(m_L.get(), "coroutine")["resume"](m_co);
}
