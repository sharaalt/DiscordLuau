/*
	@file EventDispatcher.hpp
	@author sharafzada
	@date 2026-09-05
	@brief Handles event dispatching.

	The EventDispatcher dispatchs events which are sent by discords gateway
	i.e. INTERACTION_CREATE, READY, etc and provides functions to handle the
	data.
*/

#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <functional>

class EventDispatcher {
	public:
		using Callback = std::function<void(const nlohmann::json&)>;

		void on(const std::string& eventName, Callback callback);
		void emit(const std::string& eventName, nlohmann::json& data);
	private:
		std::unordered_map<std::string, std::vector<Callback>> _listeners;
};

#endif // EVENT_DISPATCHER_HPP
