#include <GatewayClient.hpp>

GatewayClient::GatewayClient(WebSocketManager& websocket, asio::io_context& context) : _websocket(websocket), _iocontext(context), _heartbeatTimer(context) {}

void GatewayClient::connect() {
	_websocket.connect("gateway.discord.gg", "443", "/?v=10&encoding=json");

	std::string message = _websocket.receive();
	handleMessage(message);

	while (true) {
		message = _websocket.receive();
		handleMessage(message);
	}
}

void GatewayClient::handleMessage(std::string& message) {
	nlohmann::json json = nlohmann::json::parse(message);
	int op = json["op"];

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
	int heartbeatInterval = json["d"]["heartbeat_interval"];

	_heartbeatInterval = heartbeatInterval;

	this->heartbeat();
	this->identify();
}

void GatewayClient::identify() {
	nlohmann::json payload = {
		{"op", 2},
		{"d", {
			{"token", "MTU0NTYyOTA0MjM5NTQ1MTM5NA.GmHyOw.-SDn8kFT3RP0vP3Bgdj8i_R9QqXikvpJwJ4NZY"},
			{"intents", 0},
			{"properties", {
				{"os", "client_os"},
				{"browser", "client_browser"},
				{"device", "client_device"}
			}}
		}}
	};

	_websocket.send(payload.dump());
	std::string message = _websocket.receive();

	nlohmann::json json = nlohmann::json::parse(message);
	int op = json["op"];

	std::cout << op;
}

void GatewayClient::heartbeat() {
	_heartbeatTimer.expires_after(std::chrono::milliseconds(_heartbeatInterval));

	_heartbeatTimer.async_wait(
		[this](const boost::system::error_code& error) {
			if (error) {
				return;
			}

			nlohmann::json payload = {
				{"op", 1},
				{"d", nullptr}
			};

			_websocket.send(payload.dump());

			heartbeat();
		}
	);
}

void GatewayClient::handleHeartbeatAck() {}

void GatewayClient::handleDispatch(const nlohmann::json& json) {
	_sequenceNumber = json["s"];

	std::string type = json["t"];

	if (type == "READY") {
		handleReady(json["d"]);
		return;
	}
}

void GatewayClient::handleReady(const nlohmann::json& json) {
	
}