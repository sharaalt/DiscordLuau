#include <iostream>
#include <thread>

#include "Secrets.hpp"
#include "DiscordClient.hpp"

int main() {
	asio::io_context context;	
	DiscordClient manager(context);

	auto workGuard = asio::make_work_guard(context);

	std::thread networkThread([&context]() {
		context.run();
	});

	manager.connect(token);
	
	workGuard.reset();
	networkThread.join();

	return 0;
}