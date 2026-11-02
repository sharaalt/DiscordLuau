#ifndef DISCORD_BINDINGS_HPP
#define DISCORD_BINDINGS_HPP

#include <iostream>
#include <vector>
#include <memory>

#include <Types.hpp>

#include <luau/lualib.h>
#include <luau/lua.h>
#include <luau/luacode.h>

class DiscordBindings {
	public:
		void registerBindings(lua_State& _state, LuauContext* context);
};

#endif // !DISCORD_BINDINGS_HPP