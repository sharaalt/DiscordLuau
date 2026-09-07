#include "NetworkManager.hpp"

NetworkManager::NetworkManager(asio::io_context& ioContext) : _ioContext(ioContext), _sslContext(asio::ssl::context::tls_client), _resolver(ioContext), _websocket(ioContext, _sslContext) {
	//_sslContext.set_verify_mode(asio::ssl::verify_peer);
	//_sslContext.set_default_verify_paths();
}

void NetworkManager::connect(const std::string host, const std::string port, const std::string path) {
	// Setup the URL.
	auto results = _resolver.resolve(host, port);

	// Connect the websocket to the resolved URL.
	beast::get_lowest_layer(_websocket).connect(results);

	_websocket.next_layer().handshake(
		asio::ssl::stream_base::client
	);

	_websocket.handshake(host, path);	
}

HTTPResponse NetworkManager::request(beast::http::verb method, const std::string& host, const std::string& target, const std::map<std::string, std::string>& headers, const std::string& body) {
	asio::ip::tcp::resolver resolver(_ioContext);
	asio::ssl::stream<beast::tcp_stream> stream(_ioContext, _sslContext);

	beast::error_code ec;

	auto const results = resolver.resolve(host, "443", ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - Resolve failed: " << ec.message() << '\n';
		return {};
	}

	beast::get_lowest_layer(stream).connect(results, ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - TCP Connection Failed: " << ec.message() << '\n';
		return {};
	}

	stream.handshake(asio::ssl::stream_base::client, ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - TLS Handshake Failed: " << ec.message() << '\n';
		return {};
	}

	beast::http::request<beast::http::string_body> request{ method, target, 11 };

	request.set(beast::http::field::host, host);

	for (const auto& [name, value] : headers) {
		request.set(name, value);
	};

	request.body() = body;
	request.prepare_payload();

	beast::http::write(stream, request, ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - HTTP write request failed: " << ec.message() << '\n';
		return {};
	};

	beast::flat_buffer buffer;
	beast::http::response<beast::http::string_body> res;

	beast::http::read(stream, buffer, res, ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - Failed to read HTTP res: " << ec.message() << '\n';
		return {};
	};

	HTTPResponse response;

	response.statusCode = res.result_int();
	response.body = res.body();

	for (const auto& field : res) {
		response.headers[field.name_string()] = field.value();
	}

	stream.shutdown(ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - Failed to close TLS stream: " << ec.message() << '\n';
		return {};
	};

	beast::get_lowest_layer(stream).socket().close(ec);

	if (ec) {
		std::cerr << "[NetworkingManager] - Failed to close TCP socket: " << ec.message() << '\n';
	}

	return response;
}

std::string NetworkManager::receive() {
	beast::error_code ec;
	beast::flat_buffer buffer;

	_websocket.read(buffer, ec);

	// If we recieve an error code throw it.
	if (ec) {
		throw std::runtime_error("[NetworkManager] - An error has occured whilst recieving websocket packet: " + ec.message());
	}

	return beast::buffers_to_string(buffer.data());
}

void NetworkManager::asyncRecieve(std::function<void(std::string&)> callback) {
	beast::error_code ec;

	_websocket.async_read(_buffer, 
			[this, callback](beast::error_code ec, std::size_t bytes) {
				if (ec == beast::websocket::error::closed) {
					std::cout << "[NetworkManager]  - Websocket closed by peer.\n";

					auto closeCode = _websocket.reason().code;
					auto closeReason = _websocket.reason().reason;
				
					std::cout << closeCode << '\n';
					std::cout << closeReason << '\n';
				}

				// If we recieve an error code throw it.
				if (ec) {
					throw std::runtime_error("[NetworkManager] - An error has occured whilst recieving websocket packet in async: " + ec.message());
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

void NetworkManager::send(const std::string& json) {
	_websocket.text(true);
	_websocket.write(asio::buffer(json));
}

void NetworkManager::disconnect() {
	beast::error_code ec;

	_websocket.close(beast::websocket::close_code::normal, ec);

	if (ec) {
		std::cout << "[NetworingManager] - An error has occured whilst disconnecting the websocket: " << ec << '\n';
	};
}