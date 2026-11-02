/*
	@file EventDispatcherProxy.hpp
	@author sharafzada
	@date 2026-11-1
	@brief Provides additional functions for event subscription.

	The EventDispatcherProxy class provides additional functions
	for when .on() is called in the EventDispatcher i.e getCallbackId(),
	close(), etc.
*/

#ifndef EVENT_DISPATCHER_PROXY_HPP
#define EVENT_DISPATCHER_PROXY_HPP

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include <EventDispatcher.hpp>
#include <nlohmann/json.hpp>

#include "Types.hpp"

class EventDispatcher;

class EventDispatcherProxy {
	public:
		EventDispatcherProxy(const int& callbackId, std::weak_ptr<EventDispatcher> dispatcher, const std::string& eventName);

		void close();
		const int returnCallbackId();
	private:
		std::weak_ptr<EventDispatcher> _dispatcher;
		const int& _callbackId;
		const std::string& _eventName;
};

#endif // !EVENT_DISPATCHER_PROXY_HPP
