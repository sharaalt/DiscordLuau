#include <WebSocketManager.hpp>
#include <GatewayClient.hpp>

#include <iostream>

int main() {
	boost::asio::io_context ioContext;

	WebSocketManager websocket(ioContext);
	GatewayClient client(websocket, ioContext);

	client.connect();

	return 1;
};