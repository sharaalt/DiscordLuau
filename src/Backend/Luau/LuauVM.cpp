#include "LuauVM.hpp"

namespace {
	int lua_print(lua_State* L) {
		int count = lua_gettop(L);

		lua_Debug ar;

		if (!lua_getinfo(L, 1, "s", &ar)) {
			return 0;
		}
		
		std::filesystem::path path = ar.source;

		std::cout << "[" << path.filename().string() << "] - ";

		for (int i = 1; i <= count; i++) {
			int type = lua_type(L, i);

			switch (type) {
				case LUA_TSTRING:
					std::cout << lua_tostring(L, i);
					break;
				case LUA_TNUMBER:
					std::cout << lua_tonumber(L, i);
					break;
				case LUA_TBOOLEAN:
					std::cout << (lua_toboolean(L, i) ? "true" : "false");
					break;
				case LUA_TNIL:
					std::cout << "nil";
					break;
				case LUA_TTABLE:
					std::cout << "table";
					break;
				case LUA_TFUNCTION:
					std::cout << "function";
					break;
				case LUA_TUSERDATA:
					std::cout << "userdata";
					break;
				case LUA_TTHREAD:
					std::cout << "thread";
					break;
				case LUA_TLIGHTUSERDATA:
					std::cout << "lightuserdata";
					break;
				};
		
			if (i < count) {
				std::cout << " ";
			}
		};

		std::cout << '\n';

		return 0;
	}

	int lua_require(lua_State* L) {

		lua_getfield(L, LUA_REGISTRYINDEX, "ModuleLoader");
		auto* loader = static_cast<LuauModuleLoader*>(lua_touserdata(L, -1));

		lua_pop(L, 1);

		if (!loader) {
			luaL_error(L, "[LuauVM] - ModuleLoader not initalzed.");
			return LUA_NOREF;
		}

		const char* moduleName = luaL_checkstring(L, 1);

		lua_Debug ar; 

		if (!lua_getinfo(L, 1, "s", &ar)) {
			return 0;
		}

		std::cout << "[LuauVM] - Require called by: " << ar.source << '\n';

		std::string callerPath = ar.source;

		int ref = loader->load(moduleName, ar.source);

		if (ref == LUA_NOREF) {
			luaL_error(L, "[LuauVM] - Failed to load module %s", moduleName);
			return LUA_NOREF;
		}

		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

		return 1;
	}
}

LuauVM::LuauVM(BindingContext& binding): _luaState(luaL_newstate()), _binding(binding), _loader(_luaState) {
	// If there is not VM we assume an error.
	if (!_luaState) {
		std::cerr << "[LuauVM] - Failed to create luau VM.\n";
		return;
	}

	// Open the lua libararies and hook the C++ functions.
	luaL_openlibs(_luaState);
	_bindings.registerAllFuncs(_luaState, &_binding);

	// Push the ModuleLoader as light data for the require func.
	lua_pushlightuserdata(_luaState, &_loader);
	lua_setfield(_luaState, LUA_REGISTRYINDEX, "ModuleLoader");

	// Push the custom print command as a new global C func on the lua side.
	lua_pushcfunction(_luaState, lua_print, "luacprint");
	lua_setglobal(_luaState, "print");
		
	// Push the require function as a new global C func on the lua side.
	lua_pushcfunction(_luaState, lua_require, "luacrequire");
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
		std::cerr << "[LuauVM] - An issue occured while executing script: " << lua_tostring(_luaState, -1) << '\n';
		return;
	}

	if (lua_pcall(_luaState, 0, 0, 0) != 0) {
		std::cerr << "[LuauVM] - Luau error: " << lua_tostring(_luaState, -1) << '\n';
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