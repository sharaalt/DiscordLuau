#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>
#include <functional>

#include <nlohmann/json.hpp>

struct CallbackType {
	std::function<void(const nlohmann::json&)> callback;
	int callbackId;
	bool executeOnce;
};

struct IdentificationType {
	bool NotIdentified;
	bool Identifying;
	bool Identified;
};

struct HeartbeatType {
	bool HeartbeatActive = false;
	bool HeartbeatProcessing = false;
	bool HeartbeatScheduled = false;
};

#endif // TYPES_HPP