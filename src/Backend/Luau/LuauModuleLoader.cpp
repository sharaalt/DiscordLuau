#include <LuauModuleLoader.hpp>

LuauModuleLoader::LuauModuleLoader(lua_State* L) : _luaState(L) {}

int LuauModuleLoader::load(const std::string& moduleName, const std::string& path) {
	std::filesystem::path resolvedPath = _resolver.resolve(moduleName, path);

	// Check if we have a cache module first.
	int cachedRef = getCached(resolvedPath);

	// If we have a cached module return the reference to it.
	if (cachedRef != LUA_NOREF) {
		return cachedRef;
	}

	// Load file then compile.
	std::string loadedFile = loadFile(resolvedPath);
	std::vector<char> byteCode = compile(loadedFile, resolvedPath.string());

	// Execute the bytecode then cache and return a reference.
	int reference = execute(byteCode, resolvedPath.string());
	cache(resolvedPath, reference);

	return reference;
}

std::string LuauModuleLoader::loadFile(const std::filesystem::path& path) {
	// Open the file at the path.
	std::ifstream file(path);

	// If we can't open it then throw and error.
	if (!file.is_open()) {
		std::cerr << "[LuauModuleLoader] - An issue occured while opening file at path: " << path.string() << '\n';
		return "";
	}

	// Convert the file into characters then close it.
	std::string readFile(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	file.close();

	return readFile;
}

std::vector<char> LuauModuleLoader::compile(const std::string& source, const std::string& path) {
	// Extract size of the array for later use and compile into bytecode.
	size_t bytecodeSize = 0;
	char* bytecode = luau_compile(source.data(), source.size(), nullptr, &bytecodeSize);

	// If there is an issue with compiling we throw and error and the path (make it more meaningful later)!
	if (!bytecode) {
		std::cerr << "[LuauModuleLoader] - An issue occurred while compiling file at path: " << path << '\n';
		return {};
	}

	std::cout << "[LuauModuleLoader] - Compiled file at path: " << path << '\n';

	// Copy the bytecode to a vector then free the bytecode up.
	std::vector<char> bytecodeVector(bytecode, bytecode + bytecodeSize);
	free(bytecode);

	// Return the vector.
	return bytecodeVector;
}

int LuauModuleLoader::execute(const std::vector<char>& bytecode, const std::string& path) {
	// Load the bytecode.
	int results = luau_load(_luaState, path.c_str(), bytecode.data(), bytecode.size(), 0);

	// If results are not 0 we throw and error and return LUA_NOREF
	if (results != 0) {
		std::cerr << "[LuauModuleLoader] - An issue occured while executing script: " << lua_tostring(_luaState, -1) << '\n';
		return LUA_NOREF;
	}

	// Samething here, not 0 throw error and return LUA_NOREF
	if (lua_pcall(_luaState, 0, 1, 0) != 0) {
		std::cerr << "[LuauModuleLoader] - Luau error: " << lua_tostring(_luaState, -1) << '\n';
		return LUA_NOREF;
	}

	// Create a new reference and push it to the top of the stack.
	int ref = lua_ref(_luaState, -1);
	lua_pop(_luaState, 1);

	// Return the reference.
	return ref;
}

int LuauModuleLoader::getCached(const std::filesystem::path& path) {
	// Get the path as a string.
	std::string key = path.string();

	// Create a new iterator object.
	auto iterator = _moduleCache.find(key);

	// If we find a module then return the second value of it the ref.
	if (iterator != _moduleCache.end()) {
		return iterator->second;
	}

	// If not we return a LUA_NOREF.
	return LUA_NOREF;
}

void LuauModuleLoader::cache(const std::filesystem::path& path, int reference) {
	// If the module is loaded return a cerr.
	if (getCached(path) != LUA_NOREF) {
		std::cerr << "[LuauModuleLoader] - Module already loaded.";
		return;
	}

	// Push the new module to the back of the table.
	_moduleCache.emplace(path.string(), reference);
};
