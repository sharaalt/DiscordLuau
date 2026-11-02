#include <NetworkManager.hpp>

/*
	There's probably a better way to do this instead of namespaces, however I'll use this for now.
*/
namespace NetworkErrors {
	static void handle(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error has occured: " << ec.message() << '\n';
		}
	};

	static void handleClose(const boost::system::error_code ec, bool& isClosing, bool& isConneted) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error occured while closing websocket: " << ec.message() << '\n';
			return;
		}

		isClosing = false;
		isConneted = false;
		std::cout << "[NetworkManager] - Successfully closed websocket connection." << '\n';
	}

	static void handleWrite(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error occured while writing data: " << ec.message() << '\n';
			return;
		}

		std::cout << "[NetworkManager] - Successfully written data to the websocket." << '\n';
	}

	static void handleRead(const boost::system::error_code ec) {
		if (ec) {
			std::cerr << "[NetworkManager] - An error occured while reading data: " << ec.message() << '\n';
			return;
		}

		std::cout << "[NetworkManager] - Successfully read data from the websocket." << '\n';
	}
}

NetworkManager::NetworkManager(asio::io_context& context, std::shared_ptr<EventDispatcher> dispatcher) : ioContext(context), sslContext(asio::ssl::context::tls_client), _resolver(ioContext), _dispatcher(dispatcher), _websocket(ioContext, sslContext) {
	// Sets default SSL verification method and default paths.
	sslContext.set_verify_mode(asio::ssl::verify_peer);
	sslContext.set_default_verify_paths();
};

// ==================
// === Public API ===
// ==================
void NetworkManager::connect(const std::string& host, const std::string& port, const std::string& path) {	
	//if (isConnected) {
	//	std::cout << "[NetworkManager] - Websocket already connected." << '\n';
	//	return;
	//}

	// Initalize network variables.
	_host = host;
	_port = port;
	_path = path;

	// Get results and begin connection process.
	auto results = resolve(host, port);
	tcpConnection(results);

	// Emit a empty network event once connected.
	_dispatcher->emit("NetworkConnect", {});
}


void NetworkManager::asyncRead() {
	if (!isConnected) {
		std::cout << "[NetworkManager] - Cannot begin reading if connection has not been established." << '\n';
		return;
	}

	// Begins reading asynchronously (non-thread blocking).
	_websocket.async_read(_buffer, 
		[this](boost::system::error_code ec, size_t) {
			// Handle read errors.
			NetworkErrors::handleRead(ec);

			// If an error occurs stop the read.
			if (ec) {
				return;
			}

			// Convert the buffer data to a string.
			std::string bufferData = beast::buffers_to_string(_buffer.data());
		
			// Create a new nlohmann json object.
			nlohmann::json jsonObject = nlohmann::json::parse(bufferData);

			// "Consume" the buffer and the data.
			_buffer.consume(_buffer.size());

			// Begin a new read and emit data.
			_dispatcher->emit("NetworkEvent", jsonObject);
			this->asyncRead();
		}
	);
}

void NetworkManager::asyncWrite(const nlohmann::json& bufferData) {
	// Serialize the data than push it into a buffer.
	std::string serialData = bufferData.dump();
	auto asioBuffer = asio::buffer(serialData);

	// Writes data to the websocket and handles errors.
	_websocket.async_write(asioBuffer, 
		[](boost::system::error_code ec, size_t) {
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
			// If an error occurs prevent the network close event from firing.
			NetworkErrors::handleClose(ec, isClosing, isConnected);
			if (ec) return;

			_dispatcher->emit("NetworkClose", {});
		}	
	);
}

const bool NetworkManager::getIsConnected() {
	return isConnected;
}

// ==================
// == Private API ===
// ==================
asio::ip::tcp::resolver::results_type NetworkManager::resolve(const std::string& host, const std::string& port) {
	// Performs DNS lookup on path and port.
	auto results = _resolver.resolve(host, port);

	return results;
}

void NetworkManager::tcpConnection(asio::ip::tcp::resolver::results_type& results) {
	// Establishes the TCP connection.
	beast::get_lowest_layer(_websocket).async_connect(results, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
		if (ec) {
			std::cout << ec.message() << '\n';
			return;
		}

		tlsHandshake();
	});
}

void NetworkManager::tlsHandshake() {
	// Secures the TCP connection with TLS.
	_websocket.next_layer().async_handshake(asio::ssl::stream_base::client, [this](boost::system::error_code ec) {
		if (ec) {
			return;
		}

		websocketHandshake();
	});
}

void NetworkManager::websocketHandshake() {
	// Establishes websocket communication.
	_websocket.async_handshake(_host, _path, [this](boost::system::error_code ec) {
		if (ec) {
			return;
		}

		// Indicate the connection has been establish and print success.
		isConnected = true;
		std::cout << "[NetworkManager] - Websocket connection established." << '\n';

		// Begin reading operations.
		this->asyncRead();
	});
}