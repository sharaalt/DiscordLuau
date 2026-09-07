#include <LuauModuleLoader.hpp>

LuauModuleLoader::LuauModuleLoader(lua_State* L) : _luaState(L) {};

int LuauModuleLoader::load(const std::string& moduleName, const std::string& path) {
	std::filesystem::path resolvedPath = _resolver.resolve(moduleName, path);

	int cachedRef = getCached(resolvedPath);

	if (cachedRef != LUA_NOREF) {
		return cachedRef;
	}

	std::string loadedFile = loadFile(resolvedPath);
	
	std::vector<char> byteCode = compile(loadedFile, resolvedPath.string());

	int reference = execute(byteCode, resolvedPath.string());
	
	cache(resolvedPath, reference);

	return reference;
}

std::string LuauModuleLoader::loadFile(const std::filesystem::path& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "[LuauModuleLoader] - An issue occured while opening file at path: " << path.string() << '\n';
		return "";
	}

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

	// Copy the bytecode to a vector then free the bytecode up.
	std::vector<char> bytecodeVector(bytecode, bytecode + bytecodeSize);
	free(bytecode);

	return bytecodeVector;
}

int LuauModuleLoader::execute(const std::vector<char>& bytecode, const std::string& path) {
	int results = luau_load(_luaState, path.c_str(), bytecode.data(), bytecode.size(), 0);

	if (results != 0) {
		std::cerr << "[LuauModuleLoader] - An issue occured while executing script: " << lua_tostring(_luaState, -1) << '\n';
		return 0;
	}

	if (lua_pcall(_luaState, 0, 0, 0) != 0) {
		std::cerr << "[LuauModuleLoader] - Luau error: " << lua_tostring(_luaState, -1) << '\n';

		return 0;
	}

	return results;
}

int LuauModuleLoader::getCached(const std::filesystem::path& path) {
	std::string key = path.string();

	auto iterator = _moduleCache.find(key);

	if (iterator != _moduleCache.end()) {
		return iterator->second;
	}

	return LUA_NOREF;
}

void LuauModuleLoader::cache(const std::filesystem::path& path, int reference) {
	if (getCached(path) != LUA_NOREF) {
		std::cout << "[LuauModuleLoader] - Module already loaded.";
		return;
	}

	_moduleCache.emplace(path.string(), reference);
};
