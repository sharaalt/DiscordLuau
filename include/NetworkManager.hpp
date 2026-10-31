/*
	@file NetworkManager.hpp
	@author sharafzada
	@date 2026-10-19
	@brief
*/

#define NETWORK_MANAGER_HPP
#ifdef NETWORK_MANAGER_HPP

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

// "gateway.discord.gg", "443", "/?v=10&encoding=json"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace websocket = beast::websocket;

using tcp = asio::ip::tcp;

class NetworkManager {
	public:
		NetworkManager(asio::io_context& context);

		void connect(const std::string& host, const std::string& port, const std::string& path);
		void close();

		nlohmann::json asyncRead();
		void asyncWrite(const nlohmann::json& bufferData);
		
		bool getIsConnected();
	private:
		asio::io_context& ioContext;
		asio::ssl::context sslContext;
		tcp::resolver _resolver;
		
		beast::flat_buffer _buffer;
		websocket::stream<
			beast::ssl_stream<beast::tcp_stream>
		> _websocket;

		bool isConnected;
		bool isClosing;
};

#endif // NETWORK_MANAGER_HPP
