/*
	@file NetworkManager.hpp
	@author sharafzada
	@date 2026-10-19
	@brief Handles basic websocket interaction.

	The NetworkManager handles the lowest layer of networking 
	by interacting directly with any websocket/gateway i.e.
	Discord. It is not just meant for, Discord and can be
	used for other websites/applications.
*/

#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <iostream>
#include <functional>

#include <nlohmann/json.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>

#include "EventDispatcher.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

using tcp = asio::ip::tcp;

class NetworkManager {
	public:
		NetworkManager(asio::io_context& context, std::shared_ptr<EventDispatcher> dispatcher);

		void connect(const std::string& host, const std::string& port, const std::string& path);
		void close();

		void asyncRead();
		void asyncWrite(const nlohmann::json& bufferData);
		
		const bool getIsConnected();
	private:
		// Private Functions
		boost::asio::ip::tcp::resolver::results_type resolve(const std::string& host, const std::string& port);
		void tcpConnection(asio::ip::tcp::resolver::results_type& results);
		void tlsHandshake();
		void websocketHandshake();

		asio::io_context& ioContext;
		asio::ssl::context sslContext;
		tcp::resolver _resolver;
		
		beast::flat_buffer _buffer;
		websocket::stream<
			beast::ssl_stream<beast::tcp_stream>
		> _websocket;
		std::shared_ptr<EventDispatcher> _dispatcher;

		bool isConnected;
		bool isClosing;

		std::string _host;
		std::string _port;
		std::string _path;
};

#endif // NETWORK_MANAGER_HPP