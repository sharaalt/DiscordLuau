#include "EventDispatcher.hpp"

EventDispatcherProxy EventDispatcher::on(const std::string& eventName, std::function<void(const nlohmann::json&)> callback) {
	// Create's a new event if it already doesn't exist.
	auto& index = callbackCache[eventName];
	callbackId += 1;

	// Create a new callback item and assign it a unique callbackId then push it.
	CallbackType callbackItem{ callback, callbackId, false };
	EventDispatcherProxy callbackProxy{ callbackId, weak_from_this(), eventName };
	index.push_back(std::move(callbackItem));

	// Return the callbackId and print success.
	std::cout << "[EventDispatcher] - Successfully added a new callback with ID: " << callbackId << '\n';
	return callbackProxy;
};

void EventDispatcher::once(const std::string& eventName, std::function<void(const nlohmann::json&)> callback) {
	// Create's a new event if it already doesn't exist.
	auto& index = callbackCache[eventName];
	callbackId += 1;

	// Create a new callback item and assign it a unique callbackId then push it.
	CallbackType callbackItem{ callback, callbackId, true };
	EventDispatcherProxy callbackProxy{ callbackId, weak_from_this(), eventName };
	index.push_back(std::move(callbackItem));

	// Return the callbackId and print success.
	std::cout << "[EventDispatcher] - Successfully added a new callback with ID: " << callbackId << " this is a one time callback." << '\n';
}

void EventDispatcher::emit(const std::string& eventName, const nlohmann::json& data) {
	// Find the callbacks in the cache.
	auto cacheCallbacks = callbackCache.find(eventName);

	// If the event isn't already cached return.
	if (cacheCallbacks == callbackCache.end()) {
		std::cout << "[EventDispatcher] - No callbacks exist for " << eventName << "." << '\n';
		return;
	}

	// If not, get the vector of callbacks.
	auto& callbacks = cacheCallbacks->second;

	// Call all the callbacks.
	for (int i = 0; i < callbacks.size(); i++) {
		const CallbackType& callbackStruct = callbacks[i];
		callbackStruct.callback(data);

		if (callbackStruct.executeOnce) {
			callbacks.erase(callbacks.begin() + i);
			i--;
			std::cout << "[EventDispatcher] - Popped a once callback." << '\n';
		}
	};

	// Print success.
	std::cout << "[EventDispatcher] - Callbacks for " << eventName << " sucessfully called." << '\n';
};

void EventDispatcher::remove(const int callbackId, const std::string& eventName) {
	// Find the callbacks in the cache.
	auto cacheCallbacks = callbackCache.find(eventName);

	// If there's no cached event's there's no point in finding.
	if (cacheCallbacks == callbackCache.end()) {
		std::cout << "[EventDispatcher] - No callbacks exist for " << eventName << "." << '\n';
		return;
	}

	// If not, get a vector of callbacks.
	auto& callbacks = cacheCallbacks->second;

	for (int i = 0; i < callbacks.size(); i++) {
		const CallbackType& callbackStruct = callbacks[i];
		
		if (callbackStruct.callbackId == callbackId) {
			callbacks.erase(callbacks.begin() + i);
			i--;
			std::cout << "[EventDispatcher] - Callback with ID: " << callbackId << " destroyed." << '\n';
			break;
		}
	}
}