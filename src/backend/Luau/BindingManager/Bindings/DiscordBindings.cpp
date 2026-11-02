#include <luau/Bindings/DiscordBindings.hpp>
#include <DiscordClient.hpp>

namespace DiscordNative {
	static int connect(lua_State* state) {
		// Get the context and extract it.
		lua_getfield(state, LUA_REGISTRYINDEX, "LuauContext");
		auto* context = static_cast<LuauContext*>(lua_touserdata(state, -1));

		// Pop the context from the stack.
		lua_pop(state, 1);

		// If the context is a nullptr don't continue.
		if (context == nullptr) {
			luaL_error(state, "[DiscordNative] - LuauContext cannot be a nullptr.\n");
			return 0;
		}

		// Get the paramters and check if it's correct.
		int params = lua_gettop(state);

		// If there's more or less params we throw an error.
		if (params != 1) {
			luaL_error(state, "[DiscordNative] - Not enough or too many parameters we're given.\n");
			return 0;
		}
		
		// Extract the token and pass it to the discord client context.
		std::string token = luaL_checkstring(state, params);
		context->discordClient.connect(token);

		return 0;
	}

	static int disconnect(lua_State* state) {
		return 0;
	}
}

void DiscordBindings::registerBindings(lua_State& _state, LuauContext* context) {
	// Create a new table for our bindings.
	lua_createtable(&_state, 0, 2);

	// Push the context as light user data to be used later.
	lua_pushlightuserdata(&_state, context);
	lua_setfield(&_state, LUA_REGISTRYINDEX, "LuauContext");

	// Push the functions.
	lua_pushcfunction(&_state, DiscordNative::connect, "connect");
	lua_setfield(&_state, -2, "connect");

	lua_pushcfunction(&_state, DiscordNative::disconnect, "disconnect");
	lua_setfield(&_state, -2, "disconnect");

	// Set the global table.
	lua_setglobal(&_state, "__DiscordNative");
}