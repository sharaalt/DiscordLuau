#include <NetworkManager.hpp>

namespace NetworkErrors {
	void handle(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error has occured: " << ec.message() << '\n';
		}
	};

	void handleClose(const boost::system::error_code ec, bool& isClosing, bool& isConneted) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error occured while closing websocket: " << ec.message() << '\n';
			return;
		}

		isClosing = false;
		isConneted = false;
		std::cout << "[NetworkManager] - Successfully closed websocket connection." << '\n';
	}

	void handleWrite(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error occured while writing data: " << ec.message() << '\n';
			return;
		}

		std::cout << "[NetworkManager] - Successfully written data to the websocket." << '\n';
	}
}

NetworkManager::NetworkManager(asio::io_context& context) : ioContext(context), sslContext(asio::ssl::context::tls_client), _resolver(ioContext), _websocket(ioContext, sslContext) {
	// Sets default SSL verification method and default paths.
	sslContext.set_verify_mode(asio::ssl::verify_peer);
	sslContext.set_default_verify_paths();
};

void NetworkManager::connect(const std::string& host, const std::string& port, const std::string& path) {
	// Performs DNS lookup on path and port.
	auto results = _resolver.resolve(host, port);
	
	// Establishes the TCP connection.
	boost::beast::get_lowest_layer(_websocket).async_connect(results);

	// Secures the TCP connection with TLS.
	_websocket.next_layer().async_handshake(
		asio::ssl::stream_base::client
	);

	// Establishes websocket communication.
	_websocket.async_handshake(host, path);

	// Indicate the connection has been establish and print success.
	isConnected = true;
	std::cout << "[NetworkManager] - Websocket connection established." << '\n';
}


nlohmann::json NetworkManager::asyncRead() {

}

void NetworkManager::asyncWrite(const nlohmann::json& bufferData) {
	// Serialize the data than push it into a buffer.
	std::string serialData = bufferData.dump();
	auto asioBuffer = asio::buffer(serialData);

	// Writes data to the websocket and handles errors.
	_websocket.async_write(asioBuffer, 
		[](boost::system::error_code ec, size_t size) {
			NetworkErrors::handleWrite(ec);
		}
	);
}

void NetworkManager::close() {
	// If the websocket is closed return.
	if (!isConnected) {
		std::cout << "[NetworkManager] - Cannot close a closed websocket." << '\n';
		return; 
	}

	isClosing = true;

	// Closes the websocket connection gracefully.
	_websocket.async_close(
		websocket::close_code::normal,
		[this](boost::system::error_code ec) {
			NetworkErrors::handleClose(ec, isClosing, isConnected);
		}	
	);
}

bool NetworkManager::getIsConnected() {
	return isConnected;
}