/*
	@file BindingManager.hpp
	@author sharafzada
	@date 2026-11-2
	@brief Binds C++ functions to luau.

	The BindingManager class exposes certain C++ functions
	to the luau side that maybe needed i.e. websocket.conenct().
*/

#ifndef BINDING_MANAGER_HPP
#define BINDING_MANAGER_HPP

#include <iostream>
#include <vector>
#include <memory>

#include <Types.hpp>

#include <luau/Bindings/DiscordBindings.hpp>

#include <luau/lualib.h>
#include <luau/lua.h>
#include <luau/luacode.h>

class BindingManager {
	public:
		BindingManager(lua_State& state, std::weak_ptr<LuauContext> _context);

		void registerBindings();
		void registerFunction(const char* funcName, lua_CFunction function);
		void registerLibrary(const char* libName);

		bool isInitialized() const;
	private:
		// Classes
		DiscordBindings discordNative;

		// Variables
		std::weak_ptr<LuauContext> context;
		lua_State& _state;
		bool initialized{};
};

#endif // !BINDING_MANAGER_HPP