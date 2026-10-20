#include "MessageRouter.hpp"

void MessageRouter::handleCode(const int& opCode, const nlohmann::json& data) {
	// Access the OP codes handler.
	const auto handler = handlers.find(opCode);

	// If there is no assigned handler return.
	if (handler == handlers.end()) {
		std::cout << "[MessageRouter] - No handler assigned OP Code to: " << opCode << '\n';
		return;
	}

	// Pop the handler and pass the data through.
	std::cout << "[MessageRouter] - Handler at OP Code: " << opCode << " called." << '\n';
	handler->second(data);
}

void MessageRouter::addHandler(const int& opCode, Handler& handler) {
	// Create a new OP code register and add the handler.
	auto result = handlers.try_emplace(opCode, handler);

	// If second is true we successfully registered the OP code.
	if (result.second) {
		std::cout << "[MessageRouter] - Added handler to OP Code: " << opCode << '\n';
		return;
	}

	// If not the OP code already has an existing handler.
	std::cout << "[MessageRouter] - Handler already registered for OP Code: " << opCode << '\n';
}

void MessageRouter::removeHandler(const int& opCode) {
	// Erase the OP code.
	auto opCodeHandler = handlers.erase(opCode);

	// If the OP code was erased print success.
	if (opCodeHandler) {
		std::cout << "[MessageRouter] - Successfully deleted OP Code: " << opCode << '\n';
		return;
	}

	// If the OP code wasnt erased print a warning.
	std::cout << "[MessageRouter] - " << opCode << " has no registered handler." << '\n';
}