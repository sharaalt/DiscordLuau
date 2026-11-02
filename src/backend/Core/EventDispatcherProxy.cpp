#include <EventDispatcherProxy.hpp>

EventDispatcherProxy::EventDispatcherProxy(const int& callbackId, std::weak_ptr<EventDispatcher> dispatcher, const std::string& eventName) : _callbackId(callbackId), _dispatcher(dispatcher), _eventName(eventName) {}

void EventDispatcherProxy::close() {
	if (auto dispatcher = _dispatcher.lock()) {
		dispatcher->remove(_callbackId, _eventName);
	}
}

const int EventDispatcherProxy::returnCallbackId() {
	return _callbackId;
}