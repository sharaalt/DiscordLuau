/*
	@file EventDispatcher.hpp
	@author sharafzada
	@date 2026-10-16
	@brief Dispatchs events between the CXX and luau side.

	The EventDispatcher dispatchs events between the CXX and luau side
	and provides callback data once X event happens, which will get
	sent back to the luau side.
*/

#define EVENT_DISPATCHER_HPP
#ifdef EVENT_DISPATCHER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "Types.hpp"

class EventDispatcher {
	public:
		EventDispatcher() {};

		int on(const std::string& eventName, std::function<void(int)> callback);
		void emit(const std::string& eventName);
	private:
		int callbackId = 0;
		std::map<std::string, std::vector<CallbackType>> callbackCache;
};

#endif // !EVENT_DISPATCHER_HPP
