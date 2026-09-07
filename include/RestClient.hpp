/*
	@file RestClient.hpp
	@author sharafzada
	@date 2026-09-07
	@brief Handles REST API interactions.

	The RestClient.hpp header handles REST API interactions
	with the Discord API. It doesn't handle all the individual
	endpoints that is broken down into sub-classes i.e.
	RestMessageAPI, RestChannelAPI, etc.
*/

#ifndef REST_CLIENT_HPP
#define REST_CLIENT_HPP

#include "NetworkManager.hpp"
#include "RestMessageAPI.hpp"
#include "Types.hpp"

#include <map>
#include <string>
#include <iostream>

namespace asio = boost::asio;
namespace beast = boost::beast;

class RestClient {
	public:
		RestClient(const std::string& token, NetworkManager& manager);

		RestMessageAPI messages();
	private:
		HTTPResponse request(beast::http::verb method, const std::string& target, const std::string& body);
		
		NetworkManager& _networkManager;
		RestMessageAPI _messageApi;
		const std::string& _token;
};

#endif // !REST_CLIENT_HPP
