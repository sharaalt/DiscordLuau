#include <luau/LuauRuntime.hpp>

LuauRuntime::LuauRuntime(lua_State& state) : _state(state) {}

void LuauRuntime::executeBytecode(std::vector<char> bytecode, const char* chunkName) {
	if (bytecode.empty()) {
		std::cout << "[LuauRuntime] - Cannot execute empty bytecode.";
		return;
	}

	//luau_load();
}