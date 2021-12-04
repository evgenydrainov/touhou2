#pragma once
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <memory>

class Script
{
public:
	Script();

	void load(const std::string& fname);
	void update(float delta);

	// if you use a raw lua_State*, weird stuff happens with destruction order of m_L and m_co
	// keep this below LuaRefs
	std::unique_ptr<lua_State, decltype(lua_close)*> L{ luaL_newstate(), lua_close };

private:
	void mRegister();

	luabridge::LuaRef m_co{ L.get() };
	float m_timer = 0.0f;
	bool m_running = false;
};
