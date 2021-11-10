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

private:
	void mRegister();

	// if you use a raw lua_State*, weird stuff happens with destruction order of m_L and m_co
	std::unique_ptr<lua_State, decltype(lua_close)*> m_L;
	luabridge::LuaRef m_co;
};
