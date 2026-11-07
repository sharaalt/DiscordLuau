/*
	@file ModuleManager.hpp
	@author sharafzada
	@date 2026-11-2
	@brief Handles module resolving.

	The ModuleManager class manages the requiring of other
	luau modules. This is basically just the resolver when 
	require() is called it resolves the path and executes the
	appropriate bytecode.
*/

#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include <iostream>
#include <variant>
#include <memory>
#include <fstream>
#include <filesystem>
#include <string>
#include <optional>
#include <unordered_map>

#include <luau/lualib.h>
#include <luau/lua.h>
#include <luau/luacode.h>

#include <luau/LuauCompiler.hpp>
#include <luau/LuauRuntime.hpp>

class ModuleManager {
	public:
		ModuleManager(std::weak_ptr<LuauCompiler> _compiler, std::weak_ptr<LuauRuntime> _runtime);
	
		int requireModule(lua_State* state);

		// Cache Functions
		void clearCache();
	private:
		// Private Functions
		bool loadModule(std::filesystem::path& modulePath, lua_State* state);
		std::variant<int, std::filesystem::path, std::monostate> findModule(std::string& sourcePath, std::string& modulePath);
		void cacheModule(std::filesystem::path& modulePath, lua_State* state);
		std::optional<int> isCached(std::filesystem::path& modulePath);

		// Variables
		std::weak_ptr<LuauCompiler> compiler;
		std::weak_ptr<LuauRuntime> runtime;
		std::unordered_map<std::filesystem::path, int> cache;
};

namespace {
	static int StaticRequire(lua_State* state) {
		// Get the manager and extract it.
		lua_getfield(state, LUA_REGISTRYINDEX, "ModuleManager");
		auto* manager = static_cast<ModuleManager*>(lua_touserdata(state, -1));

		// Pop the manager from the stack.
		lua_pop(state, 1);

		// Check if manager is a nullptr.
		if (manager == nullptr) {
			luaL_error(state, "[StaticRequire] - ModuleManager cannot be a nullptr.\n");
			return 0;
		}

		// Return manager since the return type is also an int.
		return manager->requireModule(state);
	}
}

#endif // !MODULE_MANAGER_HPP