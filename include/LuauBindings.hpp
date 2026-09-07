/*
	@file LuauBindings.hpp
	@author sharafzada
	@date 2026-09-06
	@brief Handles function bindings.

	The LuauBindings class simply handles binding all necessary
	C++ functions into valid luau functions. This is not apart of
	the Luau VM class just to keep things clean.
*/

#ifndef LUAU_BINDINGS_HPP
#define LUAU_BINDINGS_HPP

#include "Types.hpp"
#include "DiscordClient.hpp"

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <iostream>

class LuauBindings {
	public:
		static void registerAllFuncs(lua_State* L, BindingContext* context) {
			registerDiscordClient(L, context);
		};
	private:
		static void registerDiscordClient(lua_State* L, BindingContext* context);
};

#endif // !LUAU_BINDINGS_HPP
