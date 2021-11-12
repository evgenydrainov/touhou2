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

void CreateBulletB(float x, float y, float speed, float direction, float radius, LuaRef script)
{
	auto& game = Game::getInstance();
	
}

void CreateBoss(LuaRef bossData)
{
	auto& game = Game::getInstance();
	game.engine->boss = std::make_unique<Boss>(bossData);
}

//void DoFile(const std::string& fname, lua_State* L)
//{
//	std::string path = "Scripts/" + fname;
//	if (luaL_dofile(L, path.c_str()) != LUA_OK)
//		Log(lua_tostring(L, -1));
//}

Script::Script() :
	m_L(luaL_newstate(), lua_close),
	m_co(m_L.get())
{
	luaL_requiref(m_L.get(), "_G", luaopen_base, 1);
	luaL_requiref(m_L.get(), LUA_COLIBNAME, luaopen_coroutine, 1);
	luaL_requiref(m_L.get(), LUA_MATHLIBNAME, luaopen_math, 1);
	lua_pop(m_L.get(), 3);

	mRegister();

	if (luaL_dofile(m_L.get(), "Scripts/std.lua") != LUA_OK)
		Log(lua_tostring(m_L.get(), -1));
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
	setGlobal(m_L.get(), &game.engine->player, "Player");

	m_timer += delta;
	while (m_timer > 0.0f)
	{
		if (!m_finished)
			if (!m_co.isNil())
			{
				LuaRef r = getGlobal(m_L.get(), "coroutine")["resume"](m_co);
				if (!r.cast<bool>())
					m_finished = true;
			}

		m_timer--;
	}
}

void Script::mRegister()
{
	getGlobalNamespace(m_L.get())
		.addFunction("CreateBullet", CreateBullet)
		.addFunction("CreateBulletB", CreateBulletB)
		.addFunction("CreateBoss", CreateBoss);
		//.addFunction("DoFile", DoFile);

	Player::luaRegister(getGlobalNamespace(m_L.get()));

	//Bullet::luaRegister(getGlobalNamespace(m_L.get()));

	Boss::luaRegister(getGlobalNamespace(m_L.get()));
}
