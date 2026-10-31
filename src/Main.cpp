#include <iostream>

#include "NetworkManager.hpp"
#include "EventDispatcher.hpp"
#include "MessageRouter.hpp"

int main() {
	//std::function<void(int)> func = [](int argC) {
	//	std::cout << "Hello, " << argC << "~!\n";
	//};

	//std::string eventName = "helloWorld!";

	asio::io_context context;	

	//MessageRouter router;
	//EventDispatcher eventDisp;
	NetworkManager manager(context);

	manager.connect("gateway.discord.gg", "443", "/?v=10&encoding=json");

	//const nlohmann::json data = {
	//	{ "op", 2 },
	//	{ "d" ,{
	//		{ "token", "1" },
	//		{ "d_heartbeat_interval", 10 }
	//	}}
	//};

	//std::function<void(int)> func = [&router, &data](int argC) {
	//	router.handleCode(argC, data);
	//};

	//std::function<void(const nlohmann::json&)> func2 = [](const nlohmann::json& data) {
	//	std::cout << data.dump() << '\n';
	//};

	//router.addHandler(10, func2);
	//eventDisp.on("helloWorld", func);
	//eventDisp.emit("helloWorld");

	//EventDispatcher eventDisp;
	//eventDisp.on(eventName, func);
	//eventDisp.emit(eventName);

	return 0;
}