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

#include <Types.hpp>
#include <luau/LuauCompiler.hpp>
#include <luau/LuauRuntime.hpp>
#include <luau/BindingManager.hpp>

class BackendManager {
	public:
		BackendManager(std::shared_ptr<LuauContext> _context);
		
		// Lifecycle Functions
		void initialize();
		void destroy();

		// Getter Functions
		bool isInitialized() const;
		bool isConnected() const;
	private:
		// Classes
		std::unique_ptr<BindingManager> bindings;
		LuauCompiler compiler;
		std::unique_ptr<LuauRuntime> runtime;

		// Variables
		bool vmActive{};
		bool connected{};
		std::shared_ptr<LuauContext> context;
		std::unique_ptr<lua_State, LuauStateDeleter> state = nullptr;
};

#endif // !BACKEND_MANAGER_HPP
