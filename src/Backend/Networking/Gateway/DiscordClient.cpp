#include <DiscordClient.hpp>

DiscordClient::DiscordClient(asio::io_context& context) : context(context), _websocket(context), _gateway(_websocket, context, _dispatcher) {};

void DiscordClient::connect(const std::string& token, lua_Integer& intents) {
	_token = token;
	_intents = static_cast<int>(intents);

	_gateway.connect(_token, _intents); // Connect to the gateway.
}

void DiscordClient::on(lua_State* L, const std::string& eventName, int callback) {
	_dispatcher.on(eventName, [this, L, eventName, callback](nlohmann::json data) {
		std::cout << "[DiscordClient] - Event caught calling lua callback.\n";

		lua_getref(L, callback);

		if (!lua_isfunction(L, -1)) {
			std::cerr << "[DiscordClient] - Invaild luau callback.\n";

			lua_pop(L, 1);
			return;
		}

		pushJson(L, data);

		if (lua_pcall(L, 1, 0, 0) != 0) {
			std::cerr << "[DiscordClient] - Lua callback error: " << lua_tostring(L, -1) << '\n';

			lua_pop(L, 1);
			return;
		}

		//lua_unref(L, callback);
	});
}

void DiscordClient::pushJson(lua_State* L, const nlohmann::json& data) {
	if (data.is_object()) {
		lua_newtable(L);

		for (auto& [key, child] : data.items()) {
			pushJson(L, child);
			lua_setfield(L, -2, key.c_str());
		}
	}
	else if (data.is_array()) {
		lua_newtable(L);

		int index = 1;
		for (const auto& child : data) {
			pushJson(L, child);
			lua_rawseti(L, -2, index++);
		}
	}
	else if (data.is_string()) {
		lua_pushstring(L, data.get<std::string>().c_str());
	}
	else if (data.is_boolean()) {
		lua_pushboolean(L, data.get<bool>());
	}
	else if (data.is_number()) {
		lua_pushnumber(L, data.get<double>());
	}
	else if (data.is_null()) {
		lua_pushnil(L);
	}
}