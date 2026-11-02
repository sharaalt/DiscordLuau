#include "DiscordClient.hpp"

namespace ErrorHandlers {
	static void heartbeatError(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[DiscordClient] - An error occured while handling heartbeat: " << ec.message() << '\n';
			return;
		}
	}
}

DiscordClient::DiscordClient(asio::io_context& context) : _context(context), _strand(asio::make_strand(_context)), _timer(_context), _ackTimer(context), _networkManager(context, _dispatcher) {
	this->initalize();
}

// ==================
// === Public API ===
// ==================
void DiscordClient::connect(const std::string& token) {
	// Move the token out of scope and into the client.
	_token = std::move(token);

	_dispatcher->once("NetworkConnect", [](const nlohmann::json&) {
		std::cout << "[DiscordClient] - Successfully connected bot to websocket." << '\n';
	});

	// Initalze the websocket.
	_networkManager.connect(host, port, path);
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

	_messageRouter.addHandler(1, [this](const nlohmann::json& data) {
		std::cout << data << '\n';
	});

	_messageRouter.addHandler(10, [this](const nlohmann::json& data) {
		// Retrieve the heartbeat interval and update the member.
		int heartbeatInterval = data["d"]["heartbeat_interval"];
		_heartbeatInterval = heartbeatInterval;

		this->startHeartbeat();
		this->identify();
	});

	_messageRouter.addHandler(11, [this](const nlohmann::json& data) {
		// Cancel the ack timer and confirm processing has been completed.
		heartbeatType.HeartbeatProcessing = false;
		_ackTimer.cancel();
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
				{ "$os", "NULL" },
				{ "$browser", "NULL" },
				{ "$device", "NULL" },
			}},
		}}
	};

	// Write the data to the websocket.
	_networkManager.asyncWrite(botData);
}

void DiscordClient::startHeartbeat() {
	// If the heartbeat is active no need to begin a new one.
	if (heartbeatType.HeartbeatActive) {
		std::cout << "[DiscordClient] - Heartbeat already active." << '\n';
		return;
	}

	// Set the heartbeat active to true and schedule the heartbeat.
	heartbeatType.HeartbeatActive = true;
	this->scheduleHeartbeat();
}

void DiscordClient::scheduleAckTimer() {
	_ackTimer.expires_after(std::chrono::milliseconds(_heartbeatInterval));
	_ackTimer.async_wait(asio::bind_executor(
		_strand,
		[this](const boost::system::error_code ec) {
			if (ec == asio::error::operation_aborted) return;

			if (ec) {
				std::cout << "[DiscordClient] - An error has occured: " << ec.message() << '\n';
				return;
			}

			if (heartbeatType.HeartbeatProcessing) {
				std::cout << "[DiscordClient] - OP Code 11 has not been recieved within the allotted time." << '\n';
				this->handleDisconnect();
				return;
			}
		}
	));
}

void DiscordClient::scheduleHeartbeat() {
	if (heartbeatType.HeartbeatScheduled) {
		std::cout << "[DiscordClient] - A heartbeat is already scheduled." << '\n';
		return;
	}

	heartbeatType.HeartbeatScheduled = true;
	std::cout << "[DiscordClient] - Heartbeat event scheduled." << '\n';

	// Setup timer.
	_timer.expires_after(std::chrono::milliseconds(_heartbeatInterval));
	_timer.async_wait(asio::bind_executor(
		_strand, 
		[this](const boost::system::error_code ec) {
			// Update error handling.
			if (ec == asio::error::operation_aborted) return;

			if (ec) {
				std::cerr << "[DiscordClient] - An error occured while waiting for timer: " << ec.message() << '\n';

				heartbeatType.HeartbeatScheduled = false;
				ErrorHandlers::heartbeatError(ec);
				return;
			}

			// Send the heartbeat data.
			this->sendHeartbeat();
		}
	));

}

void DiscordClient::sendHeartbeat() {
	// Create new heartbeat data and write it to the gateway.
	nlohmann::json heartbeatData = {
		{ "op", 1 },
		{ "d", _sequenceNumber },
	};

	_networkManager.asyncWrite(heartbeatData);

	// Update state variables.
	heartbeatType.HeartbeatProcessing = true;
	heartbeatType.HeartbeatScheduled = false;

	// Indicate the heartbeat has been sent.
	std::cout << "[DiscordClient] - Heratbeat sent." << '\n';

	// Schedule the next heartbeat and the ack timer.
	this->scheduleAckTimer();
	this->scheduleHeartbeat();
}

void DiscordClient::handleDisconnect() {
	if (reconnecting) {
		std::cout << "[DiscordClient] - Already reconnecting." << '\n';
		return;
	}

	// Change the heartbeat states.
	heartbeatType.HeartbeatProcessing = false;
	heartbeatType.HeartbeatScheduled = false;
	heartbeatType.HeartbeatActive = false;

	// Dump the active timer and print a reconnection message.
	_timer.cancel();
	_ackTimer.cancel();
	std::cout << "[DiscordClient] - Attempting to reconnect with websocket." << '\n';

	// Close the websocket connection & call reconnect.
	_networkManager.close();
	this->reconnect();
}

void DiscordClient::reconnect() {
	// If reconnecting is true return, we're already reconnecting.
	if (reconnecting) {
		std::cout << "[DiscordClient] - Already reconnecting." << '\n';
		return;
	}

	// Connect a dispatcher to network connect.
	_dispatcher->once("NetworkConnect", [this](const nlohmann::json&) {
		this->reconnecting = false;
	});

	// Reconnect to the websocket and set reconnecting to true.
	reconnecting = true;
	_networkManager.connect(host, port, path);
}