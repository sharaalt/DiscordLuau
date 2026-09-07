/*
	@file NetworkManager.hpp
	@author sharafzada
	@date 2026-09-04
	@brief Handles the WebSocket and message transmission.

	The NetworkManager is the lowest level of the network API and handles message
	transmission, and the WebSocket lifecycle. This does NOT handle the Discord Gateway
	Protocols, to make it more clear Discord Gateway Protocols is seperated into the
	GatewayClient class.
*/


#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

#include <Types.hpp>

#include <map>
#include <iostream>
#include <string>
#include <functional>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = asio::ip::tcp;

class NetworkManager {
	public:
		NetworkManager(asio::io_context& ioContext);

		void connect(const std::string host, const std::string port, const std::string path);
		void disconnect();

		void send(const std::string& json);
		HTTPResponse request(beast::http::verb method, const std::string& host, const std::string& target, const std::map<std::string, std::string>& headers, const std::string& body);

		std::string receive();
		void asyncRecieve(std::function<void(std::string&)> callback);
	private:
		static constexpr const char* scheme = "wss://";
		bool connected;
		beast::flat_buffer _buffer;

		asio::io_context& _ioContext;
		asio::ssl::context _sslContext;
		tcp::resolver _resolver;
		websocket::stream<
			beast::ssl_stream<beast::tcp_stream>
		> _websocket;
};

#endif // !NETWORK_MANAGER_HPP
