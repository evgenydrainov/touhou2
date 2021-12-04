#include "Script.h"

#include "Game.h"

#include "mymath.h"
#include "Log.h"

using namespace luabridge;

Bullet* CreateBullet(float x, float y, float speed, float direction, float radius, LuaRef uvs)
{
	auto& game = Game::getInstance();
	Bullet& b = game.stage->bullets.emplace_back();
	b.x = x;
	b.y = y;
	b.speed = speed;
	b.direction = direction;
	b.radius = radius;
	b.u = uvs[1];
	b.v = uvs[2];
	b.w = uvs[3];
	b.h = uvs[4];
	return &b;
}

Bullet* CreateBulletB(float x, float y, float speed, float direction, float radius, LuaRef uvs, LuaRef script)
{
	auto& game = Game::getInstance();
	Bullet& b = game.stage->bullets.emplace_back();
	b.x = x;
	b.y = y;
	b.speed = speed;
	b.direction = direction;
	b.radius = radius;
	b.u = uvs[1];
	b.v = uvs[2];
	b.w = uvs[3];
	b.h = uvs[4];
	b.co = getGlobal(script.state(), "coroutine")["create"](script);
	b.co_running = true;
	return &b;
}

Boss* CreateBoss(LuaRef bossData)
{
	auto& game = Game::getInstance();
	game.stage->boss = std::make_unique<Boss>(bossData);
	return game.stage->boss.get();
}

//void DoFile(const std::string& fname, lua_State* L)
//{
//	std::string path = "Scripts/" + fname;
//	if (luaL_dofile(L, path.c_str()) != LUA_OK)
//		Log(lua_tostring(L, -1));
//}

float GetDelta()
{
	auto& game = Game::getInstance();
	return game.stage->gameplayDelta;
}

void SetDelta(float d)
{
	auto& game = Game::getInstance();
	game.stage->gameplayDelta = d;
}

float Rand(float from, float to)
{
	auto& game = Game::getInstance();
	return game.stage->random(from, to);
}

Script::Script()
{
	luaL_requiref(L.get(), "_G", luaopen_base, 1);
	luaL_requiref(L.get(), LUA_COLIBNAME, luaopen_coroutine, 1);
	luaL_requiref(L.get(), LUA_MATHLIBNAME, luaopen_math, 1);
	lua_pop(L.get(), 3);

	mRegister();

	if (luaL_dofile(L.get(), "Scripts/std.lua") != LUA_OK)
		Log(lua_tostring(L.get(), -1));
}

void Script::load(const std::string& fname)
{
	if (luaL_dofile(L.get(), fname.c_str()) == LUA_OK)
	{
		LuaRef stage = getGlobal(L.get(), "Stage");
		m_co = getGlobal(L.get(), "coroutine")["create"](stage["Script"]);
		m_running = true;
	}
	else
	{
		Log(lua_tostring(L.get(), -1));
	}
}

void Script::update(float delta)
{
	auto& game = Game::getInstance();
	setGlobal(L.get(), &game.stage->player, "Player");

	if (m_running)
	{
		m_timer += delta;
		while (m_timer > 0.0f)
		{
			m_running = getGlobal(L.get(), "coroutine")["resume"](m_co);

			if (!m_running)
			{
				m_timer = 0.0f;
				break;
			}

			m_timer--;
		}
	}
}

void Script::mRegister()
{
	getGlobalNamespace(L.get())
		.addFunction("CreateBullet", CreateBullet)
		.addFunction("CreateBulletB", CreateBulletB)
		.addFunction("CreateBoss", CreateBoss)
		.addFunction("Rand", Rand)
		
		.beginNamespace("Game")
		.addProperty("Delta", GetDelta, SetDelta)
		.endNamespace();

	Player::luaRegister(getGlobalNamespace(L.get()));

	Bullet::luaRegister(getGlobalNamespace(L.get()));

	Boss::luaRegister(getGlobalNamespace(L.get()));
}
