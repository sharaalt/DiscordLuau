/*
	@file GatewayClient.hpp
	@author sharafzada
	@date 2026-09-04
	@brief Handles the Discord Gateway communcation.

	The GatewayClient is a higher level abstraction of the NetworkManager.
	It handles Discord API gateway protocols whilst NetworkManager 
	handles the actual WebSocket connection and message transmission.
*/

#ifndef GATEWAY_CLIENT_HPP
#define GATEWAY_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

#include <EventDispatcher.hpp>
#include <nlohmann/json.hpp>
#include <NetworkManager.hpp>
#include <iostream>
#include <string>
#include <functional>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class GatewayClient {
	public:
		GatewayClient(NetworkManager& websocket, asio::io_context& _context, EventDispatcher& dispatcher);

		void connect(const std::string& token, int& intents);
	private:
		void startRecieving();
		void handleMessage(std::string& message);

		void handleHello(const nlohmann::json& json);
		void identify();
		
		void handleReconnect();
		void onDisconnect();
		void handleInvalidSession(const nlohmann::json& data);

		void heartbeat();
		void handleHeartbeatAck();
	
		void handleDispatch(const nlohmann::json& json);
		void handleReady(const nlohmann::json& json);

		NetworkManager& _websocket;
		EventDispatcher& _dispatcher;

		asio::steady_timer _heartbeatTimer;
		asio::io_context& _iocontext;
		bool _stopReceiving = false;
		bool _heartbeatActive = true;
		int _heartbeatInterval;
		std::int64_t _sequenceNumber;

		int _intents;
		std::string _token;
		std::string _sessionId;
		std::string _resumeGatewayUrl;
};

#endif // !GATEWAY_CLIENT_HPP
