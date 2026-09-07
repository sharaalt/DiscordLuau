#include "RestClient.hpp"

RestClient::RestClient(const std::string& token, NetworkManager& manager) : _token(token), _networkManager(manager) {}

RestMessageAPI RestClient::messages() {
	nlohmann::json body = {
		{"content", "hello"}
	};

	std::cout << "ah" << '\n';

	request(beast::http::verb::post, "/api/v10/channels/1530236748620234886/messages", body.dump());

	return {};
}

HTTPResponse RestClient::request(beast::http::verb method, const std::string& target, const std::string& body) {
	std::map<std::string, std::string> headers;

	headers["Authorization"] = "Bot " + _token;
	headers["Content-Type"] = "application/json";

	return _networkManager.request(method, "discord.com", target, headers, body);
}