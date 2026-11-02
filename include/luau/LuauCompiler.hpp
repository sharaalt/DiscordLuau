/*
	@file LuauCompiler.hpp
	@author sharafzada
	@date 2026-11-1
	@brief Compiles luau files into bytecode.

	The LuauCompiler compiles luau files into readable bytecode
	for execution. However, the LuauCompiler class does NOT 
	actually execute the compiled code that is the responsibility
	of the LuauRuntime class.
*/

#ifndef LUAU_COMPILER_HPP
#define LUAU_COMPILER_HPP

#include <vector>
#include <memory>
#include <iostream>
#include <string>

#include <luau/lua.h>
#include <luau/luacode.h>

class LuauCompiler {
	public:
		LuauCompiler() = default;
	
		std::vector<char> compile(const char* source);
	private:
};

#endif // !LUAU_COMPILER_HPP
