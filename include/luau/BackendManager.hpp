/*
	@file BackendManager.hpp
	@author sharafzada
	@date 2026-11-1
	@brief High-level coordinator class.

	The BackendManager class is the high-level coordinator
	between the luau frontend and C++ backend. It manages
	classes below it i.e. EventBridge.
*/

#ifndef BACKEND_MANAGER_HPP
#define BACKEND_MANAGER_HPP

#include <iostream>
#include <memory>

#include <luau/lualib.h>
#include <luau/lua.h>
#include <luau/luacode.h>

#include <luau/LuauCompiler.hpp>
#include <luau/LuauRuntime.hpp>

void deleteConnection(lua_State* state) {
	lua_close(state);
}

class BackendManager {
	public:
		BackendManager() = default;
		
		// Lifecycle Functions
		void initialize();
		void destroy();

		// Getter Functions
		bool isInitialized() const;
		bool isConnected() const;
	private:
		// Classes
		LuauCompiler compiler;
		std::unique_ptr<LuauRuntime> runtime;

		// Variables
		bool vmActive;
		bool connected;
		std::unique_ptr<lua_State, decltype(&deleteConnection)> state;
};

#endif // !BACKEND_MANAGER_HPP
