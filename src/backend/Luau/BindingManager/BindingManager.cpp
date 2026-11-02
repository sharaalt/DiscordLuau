#include <luau/BindingManager.hpp>

BindingManager::BindingManager(lua_State& state, std::weak_ptr<LuauContext> _context) : _state(state), context(_context) {}

void BindingManager::registerBindings() {
	auto cntxt = context.lock();

	discordNative.registerBindings(_state, cntxt.get());
}

void BindingManager::registerFunction(const char* funcName, lua_CFunction func) {}

void BindingManager::registerLibrary(const char* libName) {}