#include <luau/ModuleManager.hpp>

ModuleManager::ModuleManager(std::weak_ptr<LuauCompiler> _compiler, std::weak_ptr<LuauRuntime> _runtime) : compiler(_compiler), runtime(_runtime) {}

// ==================
// === Public API ===
// ==================
int ModuleManager::requireModule(lua_State* state) {
	// Extract the module path and the source file path.
	const char* modulePathChar = luaL_checkstring(state, 1);
	lua_Debug debug;

	if (!lua_getinfo(state, 1, "s", &debug)) {
		luaL_error(state, "[ModuleManager] - Could not retrieve the source file name.\n");
		return 0;
	}

	// Check if the first paramater is nil.
	if (lua_isnil(state, 1)) {
		luaL_error(state, "[ModuleManager] - Module name cannot be nil or false.\n");
		return 0;
	}

	// Convert the char into a std::string than pass it down.
	std::string modulePathString(modulePathChar);
	std::string sourcePathString(debug.source);
	std::variant<int, std::filesystem::path, std::monostate> path = this->findModule(sourcePathString, modulePathString);

	if (std::holds_alternative<std::monostate>(path)) {
		luaL_error(state, "[ModuleManager] - The requested module does not exist.\n");
		return 0;
	}

	if (std::holds_alternative<int>(path)) {
		lua_getref(state, std::get<int>(path));
		return 1;
	}

	if (!loadModule(std::get<std::filesystem::path>(path), state)) {
		return 0;
	}

	return 1;
}

void ModuleManager::clearCache() {

}

// ==================
// == Private API ===
// ==================
bool ModuleManager::loadModule(std::filesystem::path& modulePath, lua_State* state) {
	// Open the module file.
	std::ifstream moduleFile(modulePath.string());

	// Check if the file is open.
	if (!moduleFile.is_open()) {
		std::cerr << "[ModuleManager] - An error occured while opening the module file.\n";
		return false;
	}

	// Check if the stream is good.
	if (moduleFile.bad()) {
		std::cerr << "[ModuleManager] - Stream is not good.\n";
		return false;
	}

	// Convert the contents into a string.
	std::string fileContents{
		std::istreambuf_iterator<char>(moduleFile),
		std::istreambuf_iterator<char>()
	};

	// Lock the compiler and runtime.
	auto lockedRuntime = runtime.lock();
	auto lockedCompiler = compiler.lock();

	// Validate the weak ptrs.
	if (!lockedRuntime || !lockedCompiler) {
		std::cerr << "[ModuleManager] - Runtime or compiler pointer(s) are invalid.\n";
		return false;
	}

	// Get the bytecode than execute.
	std::vector<char> bytecode = lockedCompiler->compile(fileContents.c_str());
	bool success = lockedRuntime->executeBytecode(bytecode, modulePath.string().c_str());

	// If the bytecode execution fails throw an error.
	if (!success) {
		luaL_error(state, "[ModuleManager] - An error occured while executing bytecode.\n");
		return false;
	}

	// Cache the module.
	cacheModule(modulePath, state);

	return true;
}	

std::variant<int, std::filesystem::path, std::monostate> ModuleManager::findModule(std::string& sourcePath, std::string& modulePath) {
	// Get the sourcePath.
	std::filesystem::path srcPath(sourcePath);
	std::filesystem::path parentPath = srcPath.parent_path();

	// Get the modulePath than normalize it.
	std::filesystem::path modPath(modulePath);

	// Combine the paths together than normalize.
	std::filesystem::path combinedPath = parentPath / modPath;
	std::filesystem::path normalizedPath = combinedPath.lexically_normal();

	std::cout << normalizedPath << '\n';

	// If the exstension isn't .luau add one.
	if (normalizedPath.extension() != ".luau") {
		std::cout << "[ModuleManager] - File exstension is not .luau, updating.\n";
		normalizedPath.replace_extension(".luau");
	}

	// Get the module reference if chached or a nullopt.
	std::optional<int> refId = isCached(normalizedPath);

	// Check if the module is already cached.
	if (refId != std::nullopt) {
		std::cout << "[ModuleManager] - File already cached.\n";
		return *refId;
	};

	// If the file doesn't exists return a nullopt.
	if (!std::filesystem::exists(normalizedPath)) {
		std::cout << "[ModuleManager] - File does not exist at the path.\n";
		return std::monostate{};
	}

	return normalizedPath;
}

void ModuleManager::cacheModule(std::filesystem::path& modulePath, lua_State* state) {
	// Check if the module is already cached.
	std::optional<int> cached = isCached(modulePath);

	// If it's already cached return.
	if (cached != std::nullopt) {
		std::cout << "[ModuleManager] - Module already cached.\n";
		return;
	}

	// Create a reference to the module.
	int modRef = lua_ref(state, -1);

	// Cache the module and print success.
	cache.emplace(modulePath, modRef);
	std::cout << "[ModuleManager] - Module has been cached.\n";

	return;
}

std::optional<int> ModuleManager::isCached(std::filesystem::path& modulePath) {
	std::cout << modulePath << '\n';
	auto cacheModule = cache.find(modulePath);

	if (cacheModule == cache.end()) {
		std::cout << "[ModuleManager] - No module cached at this path.\n";
		return std::nullopt;
	}

	return cacheModule->second;
}