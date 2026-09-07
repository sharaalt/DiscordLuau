/*
	@file RestMessageAPI.hpp
	@author sharafzada
	@date 2026-09-07
	@brief Handles message endpoint.

	The RestMessageAPI handles the messages endpoint
	and provides additional functions to use the endpoint.
*/

#ifndef REST_MESSAGE_API_HPP
#define REST_MESSAGE_API_HPP

#include "NetworkManager.hpp"
#include "Types.hpp"

#include <string>
#include <iostream>

class RestMessageAPI {
	public:
		HTTPResponse createMessage(const std::string& channelId, const std::string& content);
		HTTPResponse getMessage(const std::string& channelId, const std::string& messageId);
		HTTPResponse deleteMessage(const std::string& channelId, const std::string& messageId);
};

#endif // !REST_MESSAGE_API_HPP
