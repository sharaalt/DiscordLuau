/*
	@file LuauModuleResolver.hpp
	@author sharafzada
	@date 2026-09-06
	@brief Provides additional VM functions.

	Module Resolver provides the ability to use require()
	in luau scripts. To keep the Luau VM HPP file clean 
	and decoupled, I decided to make this!
*/

#ifndef LUAU_MODULE_RESOLVER_HPP
#define LUAU_MODULE_RESOLEVR_HPP

#include <Types.hpp>

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <filesystem>
#include <iostream>

class LuauModuleResolver {
	public:
		std::filesystem::path resolve(const std::string& moduleName, const std::string& path);
	private:
		bool isRelative(const std::string& moduleName);
		void addExtension(std::filesystem::path& moduleName);
};

#endif // !LUAU_MODULE_RESOLEVR_HPP
