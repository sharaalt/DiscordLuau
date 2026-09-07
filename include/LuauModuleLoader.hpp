/*
	@file LuauModuleLoader.hpp
	@author sharafzada
	@date 2026-09-06
	@brief Compiles and loads modules.

	LuauModuleLoader compiles luau scripts, caches them, and
	then eventually executes them on the Luau VM. This doesn't
	resolve the modules path that is the job of the LuauModuleResolver
	class.
*/

#ifndef LUAU_MODULE_LOADER_HPP
#define LUAU_MODULE_LOADER_HPP

#include <LuauModuleResolver.hpp>
#include "DiscordClient.hpp"

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <unordered_map>
#include <iterator>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

class LuauModuleLoader {
	public:
		LuauModuleLoader(lua_State* L);

		int load(const std::string& moduleName, const std::string& path);
	private:
		std::string loadFile(const std::filesystem::path& path);
		std::vector<char> compile(const std::string& source, const std::string& path);
		int execute(const std::vector<char>& bytecode, const std::string& path);
		int getCached(const std::filesystem::path& path);
		void cache(const std::filesystem::path& path, int reference);

		std::unordered_map<std::string, int> _moduleCache;
		LuauModuleResolver _resolver;
		lua_State* _luaState;
};

#endif // !LUAU_MODULE_LOADER_HPP
