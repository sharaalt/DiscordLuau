#include <luau/BackendManager.hpp>

void BackendManager::initialize() {
	// If the VM is active return.
	if (vmActive) {
		std::cout << "[BackendManager] - Luau VM already active, cannot start a new VM." << '\n';
		return;
	}

	// Initalze a new luau state.
	lua_State* rawState = luaL_newstate();

	// Check if allocation failed.
	if (rawState == nullptr) {
		std::cerr << "[BackendManager] - An error occurred whilst creating the luau VM state." << '\n';
		return;
	}

	// Set the unique ptr to the new state and open up the libs.
	state.reset(rawState);
	luaL_openlibs(state.get());

	runtime = std::make_unique<LuauRuntime>(*rawState);

	// Update the VM active to true and print success.
	vmActive = true;
	std::cout << "[BackendManager] - Luau VM initalized." << '\n';
}

void BackendManager::destroy() {
	// If the VM is active prevent the destroy function from being called.
	if (!vmActive) {
		std::cout << "[BackendManager] - No luau VM active, cannot call destroy." << '\n';
		return;
	}

	// Reset the VM.
	state.reset();

	// Print success and update the state lock.
	std::cout << "[BackendManager] - Luau VM closed." << '\n';
	vmActive = false;
}

bool BackendManager::isInitialized() const {
	return vmActive;
}

bool BackendManager::isConnected() const {
	return connected;
}