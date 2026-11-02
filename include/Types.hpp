#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>
#include <functional>

#include <luau/lualib.h>
#include <luau/lua.h>
#include <luau/luacode.h>
#include <nlohmann/json.hpp>

class DiscordClient;

struct CallbackType {
	std::function<void(const nlohmann::json&)> callback;
	int callbackId;
	bool executeOnce;
};

struct IdentificationType {
	bool NotIdentified;
	bool Identifying;
	bool Identified;
};

struct HeartbeatType {
	bool HeartbeatActive = false;
	bool HeartbeatProcessing = false;
	bool HeartbeatScheduled = false;
};

struct LuauStateDeleter {
	void operator()(lua_State* state) const {
		if (state) {
			lua_close(state);
		}
	}
};

struct LuauContext {
	DiscordClient& discordClient;
};

#endif // TYPES_HPP