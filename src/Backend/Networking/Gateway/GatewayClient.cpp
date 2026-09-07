#include <GatewayClient.hpp>

GatewayClient::GatewayClient(WebSocketManager& websocket, asio::io_context& context, EventDispatcher& dispatcher) : _websocket(websocket), _iocontext(context), _heartbeatTimer(context), _dispatcher(dispatcher) {};

void GatewayClient::connect(const std::string& token) {
	// Set the token and connect to the discord gateway.
	_token = token;
	std::cout << "[GatewayClient] Token length: "
		<< _token.size() << '\n';

	_websocket.connect("gateway.discord.gg", "443", "/?v=10&encoding=json");

	// Begin recieving in async to prevent blocking the thread.
	startRecieving();
}

void GatewayClient::startRecieving() {
	// Handle the message and start the next callback.
	_websocket.asyncRecieve([this](std::string& data) {
		handleMessage(data);

		startRecieving();
	});
}

void GatewayClient::handleMessage(std::string& message) {
	// Get the OP code from the sent data.
	nlohmann::json json = nlohmann::json::parse(message);
	int op = json["op"];

	std::cout << "[GatewayClient] - Recieved OP code: " << op << '\n';

	// OP code handling (could be a bit better)
	switch (op) {
		case 0:
			handleDispatch(json);
			break;
		case 7:
			break;	
		case 9:
			break;
		case 10:
			handleHello(json);
			break;
		case 11:
			handleHeartbeatAck();
			break;
	}
}

void GatewayClient::handleHello(const nlohmann::json& json) {
	// Retrieve the heartbeatInterval from the data.
	int heartbeatInterval = json["d"]["heartbeat_interval"];

	// Set the heartbeatInterval.
	_heartbeatInterval = heartbeatInterval;

	// Begin the heartbeat loop and identification.
	this->identify();
	this->heartbeat();
}

void GatewayClient::identify() {
	nlohmann::json payload = {
		{"op", 2},
		{"d", {
			{"token", _token}, // Whoopsies, add something to initalize the token don't leave it here again D-:
			{"intents", 0},
			{"properties", {
				{"os", "client_os"}, // Update this to include client data.
				{"browser", "client_browser"},
				{"device", "client_device"}
			}}
		}}
	};

	// Dump our payload and send it as a response to the discord gateway.
	_websocket.send(payload.dump());

	std::cout << "[GatewayClient] - Sucessfully conneted to the discord gateway.\n";
}

void GatewayClient::heartbeat() {
	// Setup a heartbeat timer.
	_heartbeatTimer.expires_after(std::chrono::milliseconds(_heartbeatInterval));

	// Once it expires restart and send a new heartbeat.
	_heartbeatTimer.async_wait(
		[this](const boost::system::error_code& error) {
			if (error) {
				return;
			}

			nlohmann::json payload = {
				{"op", 1},
				{"d", nullptr}
			};

			// Send the payload to the gateway to signify we're still alive!
			_websocket.send(payload.dump());

			// Begin a new timer and print success.
			std::cout << "[GatewayClient] - Sucessfully sent heartbeat.\n";
			heartbeat();
		}
	);
}

void GatewayClient::handleHeartbeatAck() {
	std::cout << "[GatewayClient] - Sucessfully recieved heartbeat ACK.\n";
}

void GatewayClient::handleDispatch(const nlohmann::json& json) {
	// Get the sequencenumber and the dispatch type.
	_sequenceNumber = json["s"];

	std::string type = json["t"];
	auto data = json["d"];

	_dispatcher.emit(type, data);
}

void GatewayClient::handleReady(const nlohmann::json& json) {
	
}
