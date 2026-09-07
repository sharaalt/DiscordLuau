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

#include "DiscordClient.hpp"

struct BindingContext {
	DiscordClient& client;
};

#endif // !TYPES_HPP