#include <iostream>
#include <thread>

#include "Secrets.hpp"
#include "DiscordClient.hpp"
#include "luau/BackendManager.hpp"

int main() {
	lua_State* L = nullptr;

	asio::io_context context;	
	DiscordClient manager(context);

	std::shared_ptr<LuauContext> cntxt = std::make_shared<LuauContext>(manager);
	BackendManager luauVm(cntxt);

	luauVm.initialize();

	auto workGuard = asio::make_work_guard(context);

	std::thread networkThread([&context]() {
		context.run();
	});

	manager.connect(token);
	
	workGuard.reset();
	networkThread.join();

	return 0;
}