/*
	@file Main.cpp
	@author sharafzada
	@date 2026-09-04
	@brief The main entry point of the file.

	Currently just used as a testing bed.
*/

#include <luacode.h>
#include <lua.h>
#include <lualib.h>

#include <LuauVM.hpp>
#include <Types.hpp>
#include <DiscordClient.hpp>
#include <NetworkManager.hpp>
#include <GatewayClient.hpp>

#include <iostream>

DiscordClient* client = nullptr;

int main() {
	// Create a new ioContext and discord client.
	boost::asio::io_context ioContext;
	client = new DiscordClient(ioContext);

	// Create a new context and VM instance and run it up.
	BindingContext context{ *client };
	LuauVM luauVM{context};

	luauVM.runScript("scripts/main.luau");

	// Run the IO context.
	ioContext.run();

	return 0;
};