/*
	@file EventDispatcher.hpp
	@author sharafzada
	@date 2026-10-19
	@brief Routes JSON messages to the relavent locations.

	The MessageRouter routes JSON messages by using its OP Code
	and looking it up in a map to find the binded callback. Once
	found, it "pops" the callback and returns the data to the 
	callback.
*/

#ifndef MESSAGE_ROUTER_HPP
#define MESSAGE_ROUTER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include <nlohmann/json.hpp>

using Handler = std::function<void(const nlohmann::json&)>;

class MessageRouter {
	public:
		MessageRouter() {};
		~MessageRouter() {};

		void handleCode(const int& opCode, const nlohmann::json& data);

		void addHandler(const int& opCode, Handler handler);
		void removeHandler(const int& opCode);
	private:
		std::unordered_map<int, Handler> handlers;
};

#endif // MESSAGE_ROUTER_HPP