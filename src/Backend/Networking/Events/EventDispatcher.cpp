#include <EventDispatcher.hpp>

void EventDispatcher::on(const std::string& eventName, Callback callback) {
	// Push the listener to the back of the event.
	_listeners[eventName].push_back(std::move(callback));
};

void EventDispatcher::emit(const std::string& eventName, nlohmann::json& data) {
	// Retrieve the event data.
	auto eventData = _listeners.find(eventName);

	// If the table is empty return.
	if (eventData == _listeners.end()) {
		return;
	}

	// Call all callbacks for that event.
	for (auto& callback : eventData->second) {
		callback(data);
	}
}