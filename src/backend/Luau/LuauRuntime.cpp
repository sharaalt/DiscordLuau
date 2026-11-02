#include <luau/LuauRuntime.hpp>

LuauRuntime::LuauRuntime(lua_State& state) : _state(state) {}

void LuauRuntime::executeBytecode(std::vector<char> bytecode, const char* chunkName) {
	// If the vector is empty there's no point in executing empty bytecode.
	if (bytecode.empty()) {
		std::cout << "[LuauRuntime] - Cannot execute empty bytecode.";
		return;
	}

	// If the loaded bytecode is not LUA_OK that means an error has occured.
	if (luau_load(&_state, chunkName, bytecode.data(), bytecode.size(), 0) != LUA_OK) {
		// Extract the error message.
		const char* errorMessage = lua_tostring(&_state, -1);

		// If the error is a nullptr theres nothing to print.
		if (errorMessage == nullptr) {
			std::cerr << "[LuauRuntime] - An error occured, however no error message was extracted." << '\n';
			return;
		}

		// Return and throw the error message.
		std::cerr << "[LuauRuntime] - An error occured while loading bytecode: " << errorMessage << '\n';
		lua_error(&_state);

		lua_pop(&_state, 1);

		return;
	};

	// If the pcall function doesn't return LUA_OK an error has occured.
	if (lua_pcall(&_state, 0, 0, 0) != LUA_OK) {
		// Extract the error message.
		const char* errorMessage = lua_tostring(&_state, -1);
		lua_pop(&_state, 1);

		// If the error is a nullptr theres nothing to print.
		if (errorMessage == nullptr) {
			std::cerr << "[LuauRuntime] - An error occured, however no error message was extracted." << '\n';
			return;
		}

		// Return and throw the error message.
		std::cerr << "[LuauRuntime] - An error occured while executing bytecode: " << errorMessage << '\n';
		lua_error(&_state);

		lua_pop(&_state, 1);

		return;
	}
}