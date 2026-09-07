/*
	@file DiscordClient.hpp
	@author sharafzada
	@date 2026-09-05
	@brief Exposes certain WebScoket functions to the luau layer.

	The DiscordClient Class exposes certain WebSocket functions from GatewayClient.hpp
	and sends them down to the luau client. The DiscordClient class does not expose 
	everything only what the luau client needs to know i.e. when an event happened and
	the relevant data.
*/

#ifndef DISCORD_CLIENT_HPP
#define DISCORD_CLIENT_HPP

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

#include <RestClient.hpp>
#include <EventDispatcher.hpp>
#include <GatewayClient.hpp>
#include <NetworkManager.hpp>

#include <iostream>
#include <string>

class DiscordClient {
	public:
		DiscordClient(asio::io_context& ioContext);

		void pushJson(lua_State* L, const nlohmann::json& data);

		void connect(const std::string& token, lua_Integer& intents);
		void on(lua_State* L, const std::string& eventName, int callback);
	private:
		asio::io_context& context;
		EventDispatcher _dispatcher;
		NetworkManager _websocket;
		GatewayClient _gateway;
		RestClient _rest;

		int _intents;
	    std::string _token;
};

#endif // !DISCORD_CLIENT_HPP
