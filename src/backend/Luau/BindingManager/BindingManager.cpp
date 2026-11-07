#include <luau/BindingManager.hpp>

BindingManager::BindingManager(lua_State& state, std::weak_ptr<LuauContext> _context) : _state(state), context(_context) {}

void BindingManager::registerBindings() {
	// Lock the context.
	auto cntxt = context.lock();

	// Register bindings with the locked context.
	discordNative.registerBindings(_state, cntxt.get());

	// Print a success message.
	std::cout << "[BindingManager] - Successfully registered bindings." << '\n';
	return;
}

void BindingManager::registerFunction(const char* funcName, lua_CFunction func) {
	// Get the global by the funcname than pop it from the stack.
	int funcExists = lua_getglobal(&_state, funcName);
	lua_pop(&_state, 1);

	// If the global is assigned don't overwrite it.
	if (funcExists != LUA_TNIL) {
		std::cerr << "[BindingManager] - Global already exists with the name " << funcName << '\n';
		return;
	}

	// Push a new function and set the global.
	lua_pushcfunction(&_state, func, funcName);
	lua_setglobal(&_state, funcName);

	// Print a success messsage than return.
	std::cout << "[BindingManager] - Assigned a new global func with the name: " << funcName << '\n';
	return;
}

void BindingManager::registerLibrary(const char* libName) {}