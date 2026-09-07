/*
	@file LuauVM.hpp
	@author sharafzada
	@date 2026-09-06
	@brief Handles luau VM interactions.

	The luau VM class handles interactions between the C++ side and luau
	side as well as provides functions for luau such as require(). Luau VM
	only acts as a basic configuration and link to luau and does NOT handle
	additional functions however provides them from seperate classes which
	is owned by this class i.e. LuauBindings.hpp, LuauModuleResolver.hpp, etc.
*/

#ifndef LUAU_VM_HPP
#define LUAU_VM_HPP

#include <LuauModuleLoader.hpp>
#include <LuauBindings.hpp>
#include <Types.hpp>

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <vector>
#include <iostream>

class LuauVM {
	public:
		LuauVM(BindingContext& binding);
		~LuauVM();

		void runScript(const std::filesystem::path& path);
		lua_State* state();
	private:
		std::string loadFile(const std::filesystem::path& path);
		std::vector<char> compile(const std::string& source, const std::string& path);

		LuauModuleLoader _loader;
		LuauBindings _bindings;
		BindingContext& _binding;
		lua_State* _luaState;
};

#endif // LUAU_VM_HPP
