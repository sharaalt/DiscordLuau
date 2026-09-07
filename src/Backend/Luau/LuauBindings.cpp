#include "LuauBindings.hpp"

namespace {
	int discord_connect_func(lua_State* L) {
		const char* token = luaL_checkstring(L, 1);

		lua_getfield(L, LUA_REGISTRYINDEX, "BindingContext");

		auto* context = static_cast<BindingContext*>(lua_touserdata(L, -1));

		context->client.connect(token);

		return 0;
	}

	int discord_on_event_func(lua_State* L) {
		luaL_checktype(L, 2, LUA_TFUNCTION);

		lua_getfield(L, LUA_REGISTRYINDEX, "BindingContext");

		auto* context = static_cast<BindingContext*>(lua_touserdata(L, -1));

		const std::string eventName = luaL_checkstring(L, 1);
		int callbacRef = lua_ref(L, 2);

		context->client.on(L, eventName, callbacRef);

		return 0;
	}
}

void LuauBindings::registerDiscordClient(lua_State* L, BindingContext* context) {
	lua_newtable(L);

	lua_pushlightuserdata(L, context);
	lua_setfield(L, LUA_REGISTRYINDEX, "BindingContext");

	lua_pushcfunction(L, discord_connect_func, "discordconnectfunctionc");
	lua_setfield(L, -2, "connect");

	lua_pushcfunction(L, discord_on_event_func, "discordeventfunctionc");
	lua_setfield(L, -2, "on");

	lua_setglobal(L, "__DiscordNative");
};