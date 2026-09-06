#include <DiscordClient.hpp>

DiscordClient::DiscordClient(asio::io_context& context) : context(context), _websocket(context), _gateway(_websocket, context, _dispatcher) {};

void DiscordClient::connect(const std::string& token) {
	_token = token;

	_gateway.connect(_token); // Connect to the gateway.
}

void DiscordClient::on(const std::string* eventName, int callback) {

}