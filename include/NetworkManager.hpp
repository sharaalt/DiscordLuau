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

#include <boost/beast.hpp>
#include <boost/asio.hpp>

// "gateway.discord.gg", "443", "/?v=10&encoding=json"

namespace asio = boost::asio;
namespace beast = boost::beast;

using tcp = asio::ip::tcp;

class NetworkManager {
	public:
		void connect();
		void close();

		void asyncRead();
		void asyncWrite();
		
		bool isConnected();
	private:
		bool isConnected;
};

#endif // NETWORK_MANAGER_HPP
