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
	beast::error_code ec;
	beast::flat_buffer buffer;

	_websocket.read(buffer, ec);

	// If we recieve an error code throw it.
	if (ec) {
		throw std::runtime_error("[WebSocketManager] - An error has occured whilst recieving websocket packet: " + ec.message());
	}

	return beast::buffers_to_string(buffer.data());
}

void WebSocketManager::asyncRecieve(std::function<void(std::string&)> callback) {
	beast::error_code ec;

	_websocket.async_read(_buffer, 
			[this, callback](beast::error_code ec, std::size_t bytes) {
				if (ec == beast::websocket::error::closed) {
					std::cout << "[WebSocketManager]  - Websocket closed by peer.\n";

					auto closeCode = _websocket.reason().code;
					auto closeReason = _websocket.reason().reason;
				
					std::cout << closeCode << '\n';
					std::cout << closeReason << '\n';
				}

				// If we recieve an error code throw it.
				if (ec) {
					throw std::runtime_error("[WebSocketManager] - An error has occured whilst recieving websocket packet in async: " + ec.message());
				}
				
				// Convert the buffer to a string.
				std::string message = beast::buffers_to_string(_buffer.data());

				// Clean up the buffer.
				_buffer.consume(_buffer.size());

				// Return the message via callback.
				callback(message);
			}
		);
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