#include "EventDispatcher.hpp"

int EventDispatcher::on(const std::string& eventName, std::function<void(int)> callback) {
	// Create's a new event if it already doesn't exist.
	auto& index = callbackCache[eventName];
	callbackId += 1;

	// Create a new callback item and assign it a unique callbackId then push it.
	CallbackType callbackItem{ callback, callbackId };
	index.push_back(std::move(callbackItem));

	// Return the callbackId and print success.
	std::cout << "[EventDispatcher] - Successfully added a new callback with ID: " << callbackId << '\n';
	return callbackId;
};

void EventDispatcher::emit(const std::string& eventName) {
	// Find the callbacks in the cache.
	auto cacheCallbacks = callbackCache.find(eventName);

	// If the event isn't already cached return.
	if (cacheCallbacks == callbackCache.end()) {
		std::cout << "[EventDispatcher] - No callbacks exist for " << eventName << "." << '\n';
		return;
	}

	// If not, get the vector of callbacks.
	const auto& callbacks = cacheCallbacks->second;

	// Call all the callbacks.
	for (auto& callbackStruct : callbacks) {
		callbackStruct.callback(10);
	};

	// Print success.
	std::cout << "[EventDispatcher] - Callbacks for " << eventName << " sucessfully called." << '\n';
};