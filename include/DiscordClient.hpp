/*
	@file DiscordClient.hpp
	@author sharafzada
	@date 2026-10-31
	@brief Handles high level networking.

	The DiscordClient ties all the network classes together i.e. MessageRouter,
	NetworkManager, etc and uses a unified class to manage all of them. This 
	acts as the final connection before exposing to the API to the luau side.
	
	@note Could use refactoring/decoupling it's a bit beefy for my liking.
*/

#ifndef DISCORD_CLIENT_HPP
#define DISCORD_CLIENT_HPP

#include <memory>

#include "EventDispatcher.hpp"
#include "NetworkManager.hpp"
#include "MessageRouter.hpp"

class DiscordClient {
	public:
		DiscordClient(asio::io_context& context);

		void connect(const std::string& token);
		void disconnect();
	private:
		// Private Funcs
		void initalize();
		void identify();
		void startHeartbeat();
		void scheduleAckTimer();
		void scheduleHeartbeat();
		void sendHeartbeat();
		void handleDisconnect();
		void reconnect();

		// Gateway Data
		const std::string host = "gateway.discord.gg";
		const std::string port = "443";
		const std::string path = "/?v=10&encoding=json";
		
		// Variables
		asio::io_context& _context;
		asio::strand<asio::io_context::executor_type> _strand;
		asio::steady_timer _timer;
		asio::steady_timer _ackTimer;
		std::string _token;

		int _heartbeatInterval;
		int _sequenceNumber = NULL;
		bool identified = false;
		bool reconnecting = false;

		HeartbeatType heartbeatType;

		// Dependencies
		std::shared_ptr<EventDispatcher> _dispatcher = std::make_shared<EventDispatcher>();
		MessageRouter _messageRouter;
		NetworkManager _networkManager;
};

#endif // DISCORD_CLIENT_HPP
