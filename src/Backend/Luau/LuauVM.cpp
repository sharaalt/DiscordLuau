#include "LuauVM.hpp"

namespace {
	int lua_require(lua_State* L) {

		lua_getfield(L, LUA_REGISTRYINDEX, "ModuleLoader");
		auto* loader = static_cast<LuauModuleLoader*>(lua_touserdata(L, -1));

		lua_pop(L, 1);

		if (!loader) {
			luaL_error(L, "[LuaVM] - ModuleLoader not initalzed.");
			return 0;
		}

		const char* moduleName = luaL_checkstring(L, 1);

		lua_Debug ar;

		if (!lua_getinfo(L, 1, "s", &ar)) {
			return 0;
		}

		std::cout << "[LuaVM] - Require called by: " << ar.source << '\n';

		std::string callerPath = ar.source;

		loader->load(moduleName, ar.source);

		return 0;
	}
}

LuauVM::LuauVM(BindingContext& binding): _luaState(luaL_newstate()), _binding(binding), _loader(_luaState) {
	if (!_luaState) {
		std::cerr << "[LuaVM] - Failed to create luau VM\n";
		return;
	}

	luaL_openlibs(_luaState);
	_bindings.registerAllFuncs(_luaState, &_binding);

	lua_pushlightuserdata(_luaState, &_loader);
	lua_setfield(_luaState, LUA_REGISTRYINDEX, "ModuleLoader");

	lua_pushcfunction(_luaState, lua_require, "require");
	lua_setglobal(_luaState, "require");
}

LuauVM::~LuauVM() {
	lua_close(_luaState);
}

void LuauVM::runScript(const std::filesystem::path& path) {
	std::string source = loadFile(path);

	if (source.empty()) {
		return;
	}

	std::string pathString = path.string();
	std::vector<char> bytecode = compile(source, pathString);

	int results = luau_load(_luaState, pathString.c_str(), bytecode.data(), bytecode.size(), 0);

	if (results != 0) {
		std::cerr << "[LuauModuleLoader] - An issue occured while executing script: " << lua_tostring(_luaState, -1) << '\n';
		return;
	}

	if (lua_pcall(_luaState, 0, 0, 0) != 0) {
		std::cerr << "[LuauModuleLoader] - Luau error: " << lua_tostring(_luaState, -1) << '\n';
		return;
	}
}

std::string LuauVM::loadFile(const std::filesystem::path& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "[LuauVM] - An issue occured while opening file at path: " << path.string() << '\n';
		return "";
	}

	std::string readFile(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	file.close();

	return readFile;
}

std::vector<char> LuauVM::compile(const std::string& source, const std::string& path) {
	// Extract size of the array for later use and compile into bytecode.
	size_t bytecodeSize = 0;
	char* bytecode = luau_compile(source.data(), source.size(), nullptr, &bytecodeSize);

	// If there is an issue with compiling we throw and error and the path (make it more meaningful later)!
	if (!bytecode) {
		std::cerr << "[LuauVM] - An issue occurred while compiling file at path: " << path << '\n';
		return {};
	}

	// Copy the bytecode to a vector then free the bytecode up.
	std::vector<char> bytecodeVector(bytecode, bytecode + bytecodeSize);
	free(bytecode);

	return bytecodeVector;
}

lua_State* LuauVM::state() {
	return _luaState;
}