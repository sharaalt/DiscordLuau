/*
	@file Main.hpp
	@author sharafzada
	@date 2026-09-04
	@brief The main entry point of the file.

	Currently just used as a testing bed.
*/

#include <WebSocketManager.hpp>
#include <GatewayClient.hpp>

#include <iostream>

int main() {
	boost::asio::io_context ioContext;

	WebSocketManager websocket(ioContext);
	GatewayClient client(websocket, ioContext); // Pass onto luau for later use.

	client.connect();

	return 1;
};