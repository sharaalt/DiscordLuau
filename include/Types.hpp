/*
	@file Types.hpp
	@author sharafzada
	@date 2026-09-06
	@brief Provides additional types.

	Types.hpp provides additional types for the C++
	side thats about it.
*/

#ifndef TYPES_HPP
#define TYPES_HPP

#include <map>
#include <string>
#include <iostream>

class DiscordClient;

struct HTTPResponse {
	int statusCode;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct BindingContext {
	DiscordClient& client;
};

#endif // !TYPES_HPP