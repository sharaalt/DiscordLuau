#include "WebSocketManager.hpp"

WebSocketManager::WebSocketManager(asio::io_context& ioContext) : _ioContext(ioContext), _sslContext(asio::ssl::context::tls_client), _resolver(ioContext), _websocket(ioContext, _sslContext) {}

void WebSocketManager::connect(const std::string host, const std::string port, const std::string path) {
	// Setup the URL.
	auto results = _resolver.resolve(host, port);

	// Connect the websocket to the resolved URL.
	beast::get_lowest_layer(_websocket).connect(results);

	_websocket.next_layer().handshake(
		asio::ssl::stream_base::client
	);

	_websocket.handshake(host, path);
}

std::string WebSocketManager::receive() {
	beast::flat_buffer buffer;

	_websocket.read(buffer);

	return beast::buffers_to_string(buffer.data());
}

void WebSocketManager::send(const std::string& json) {
	_websocket.text(true);
	_websocket.write(asio::buffer(json));
}

void WebSocketManager::disconnect() {
	beast::error_code ec;

	_websocket.close(beast::websocket::close_code::normal, ec);

	if (ec) {
		std::cout << "An error has occured whilst disconnecting the websocket: " << ec << '\n';
	};
}