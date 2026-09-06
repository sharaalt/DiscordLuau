/*
	@file Main.hpp
	@author sharafzada
	@date 2026-09-04
	@brief The main entry point of the file.

	Currently just used as a testing bed.
*/

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <DiscordClient.hpp>
#include <WebSocketManager.hpp>
#include <GatewayClient.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

DiscordClient* client = nullptr;

int discord_connect_func(lua_State* L) {
	const char* token = luaL_checkstring(L, 1);

	client->connect(token);

	return 0;
}

int openDiscord(lua_State* L) {
	lua_newtable(L);

	lua_pushcfunction(L, discord_connect_func, "discordconnectfunctionc");
	lua_setfield(L, -2, "connect");

	lua_setglobal(L, "__DiscordNative");

	return 0;
}

std::string loadFileSource(const char* modname) {
	std::string path = "scripts/";

	std::string moduleName(modname);

	if (moduleName.starts_with('./')) {
		moduleName = moduleName.substr(2);
	}

	path += moduleName;

	if (!moduleName.ends_with("luau")) {
		path += ".luau";
	}

	std::ifstream file(path);

	if (!file) {
		std::cerr << "[Main] - Failed to open scripts/main.luau\n";
		return 0;
	}


	return std::string(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
};

int lua_require(lua_State* L) {
	const char* moduleaname = luaL_checkstring(L, 1);
	
	lua_rawgetfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, -1, moduleaname);

	if (!lua_isnil(L, -1)) {
		return 1;
	}

	lua_pop(L, 2);

	std::string source = loadFileSource(moduleaname);

	size_t bytecodeSize = 0;
	char* bytecode = luau_compile(source.data(), source.size(), nullptr, &bytecodeSize);

	int results = luau_load(L, "scripts/main.luau", bytecode, bytecodeSize, 0);

	free(bytecode);

	if (results != 0) {
		std::cerr << "[Main] - Failed to load Luau script: " << lua_tostring(L, -1) << '\n';
		return 0;
	}

	if (lua_pcall(L, 0, 1, 0) != 0) {
		std::cerr << "[Main] - Luau error: " << lua_tostring(L, -1) << '\n';
		return 0;
	}

	lua_rawgetfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_pushvalue(L, -2);
	lua_setfield(L, -2, moduleaname);
	lua_pop(L, 1);

	return 1;
}

int main() {
	boost::asio::io_context ioContext;

	client = new DiscordClient(ioContext);

	lua_State* L = luaL_newstate();

	if (!L) {
		std::cerr << "[Main] - Failed to create luau VM\n";
		return 1;
	}

	// Load libraries
	openDiscord(L);
	luaL_openlibs(L);
	lua_pushcfunction(L, lua_require, "require");
	lua_setglobal(L, "require");

	//std::cout << std::filesystem::current_path();

	std::ifstream file("scripts/main.luau");
	
	if (!file) {
		std::cerr << "[Main] - Failed to open scripts/main.luau\n";
		return 0;
	}

	std::string source(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	size_t bytecodeSize = 0;

	char* bytecode = luau_compile(source.data(), source.size(), nullptr, &bytecodeSize);

	int results = luau_load(L, "scripts/main.luau", bytecode, bytecodeSize, 0);

	free(bytecode);

	if (results != 0) {
		std::cerr << "[Main] - Failed to load Luau script: " << lua_tostring(L, -1) << '\n';

		lua_close(L);
		return 0;
	}

	if (lua_pcall(L, 0, 0, 0) != 0) {
		std::cerr << "[Main] - Luau error: " << lua_tostring(L, -1) << '\n';

		lua_close(L);
		return 0;
	}

	//WebSocketManager websocket(ioContext);
	//GatewayClient client(websocket, ioContext); // Pass onto luau for later use.

	//client.connect("MTU0NTYyOTA0MjM5NTQ1MTM5NA.GebzP1.hilwsgFDcS5Fuv5_FCeBM_1XH4vzUmKofDfJaw");

	ioContext.run();

	return 0;
};