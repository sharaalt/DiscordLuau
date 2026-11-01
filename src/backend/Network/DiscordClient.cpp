#include "DiscordClient.hpp"

namespace ErrorHandlers {
	static void heartbeatError(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[DiscordClient] - An error occured while handling heartbeat: " << ec.message() << '\n';
			return;
		}
	}
}

DiscordClient::DiscordClient(asio::io_context& context) : _context(context), _timer(_context), _networkManager(context, _dispatcher) {
	this->initalize();
}

// ==================
// === Public API ===
// ==================
void DiscordClient::connect(const std::string& token) {
	// Move the token out of scope and into the client.
	_token = std::move(token);

	// Initalze the websocket.
	_networkManager.connect(host, port, path);

	//// Write the data to the websocket.
	//_networkManager.asyncWrite(data);

	std::cout << "[DiscordClient] - Successfully connected bot to websocket." << '\n';
}

void DiscordClient::disconnect() {

}

// ==================
// === Private API ==
// ==================
void DiscordClient::initalize() {
	// Event Dispatcher Handlers
	_dispatcher->on("NetworkEvent", [this](const nlohmann::json& data) {
		std::cout << "[DiscordClient] - Data recieved from websocket." << '\n';

		// Get the OP code and handle it.
		int opCode = data["op"];
		_messageRouter.handleCode(opCode, data);
	});

	// OP Code Handlers
	_messageRouter.addHandler(0, [this](const nlohmann::json& data) {
		std::cout << "[DiscordClient] - Event dispatched." << '\n';
		
		// Retrieve the "t" field.
		const std::string tField = data["t"];

		// Retrieve the new sequence number.
		int sequenceNumber = data["s"];
		_sequenceNumber = sequenceNumber;

		if (tField == "READY") {
			identified = true;
		};
	});

	_messageRouter.addHandler(10, [this](const nlohmann::json& data) {
		// Retrieve the heartbeat interval and update the member.
		int heartbeatInterval = data["d"]["heartbeat_interval"];
		_heartbeatInterval = heartbeatInterval;

		this->startHeartbeat();
		this->identify();

		//_networkManager.asyncWrite(botData);
	});

	_messageRouter.addHandler(11, [this](const nlohmann::json& data) {
		std::cout << data << '\n';
	});
}

void DiscordClient::identify() {
	// If already identified return.
	if (identified) {
		std::cout << "[DiscordClient] - Already identified." << '\n';
		return;
	}

	// Create the basic identification payload.
	nlohmann::json botData = {
		{ "op", 2, },
		{ "d",{
			{ "token", _token },
			{ "intents", 0 },
			{ "properties",{
				{ "os", "NULL" },
				{ "browser", "NULL" },
				{ "device", "NULL" },
			}},
		}}
	};

	// Write the data to the websocket.
	_networkManager.asyncWrite(botData);
}

void DiscordClient::startHeartbeat() {
	// If the heartbeat is active no need to begin a new one.
	if (heartbeatActive) {
		std::cout << "[DiscordClient] - Heartbeat already active." << '\n';
		return;
	}

	// Set the heartbeat active to true and begin the heartbeat.
	heartbeatActive = true;
	_timer.expires_after(std::chrono::milliseconds(_heartbeatInterval));

	_timer.async_wait([this](const boost::system::error_code ec) {
		if (ec) {
			heartbeatActive = false;
			ErrorHandlers::heartbeatError(ec);
			return;
		}
		else if (!ec) {
			this->startHeartbeat();
		}

		// Create new heartbeat data and write it to the gateway.
		nlohmann::json heartbeatData = {
			{ "op", 1 },
			{ "d", _sequenceNumber },
		};

		_networkManager.asyncWrite(heartbeatData);
	});
}