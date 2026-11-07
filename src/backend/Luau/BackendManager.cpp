#include <luau/BackendManager.hpp>

BackendManager::BackendManager(std::shared_ptr<LuauContext> _context) : context(_context) {}

// =================
// === Public API == 
// =================
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

	// Set up the bindings class also.
	bindings = std::make_unique<BindingManager>(*state.get(), std::weak_ptr<LuauContext>(context));
	runtime = std::make_unique<LuauRuntime>(*rawState);
	manager = std::make_unique<ModuleManager>(std::weak_ptr<LuauCompiler>(compiler), std::weak_ptr<LuauRuntime>(runtime));

	// Store global variables.
	lua_pushlightuserdata(state.get(), &manager);
	lua_setfield(state.get(), LUA_REGISTRYINDEX, "ModuleManager");

	// Register the bindings.
	bindings->registerBindings();
	bindings->registerFunction("require", StaticRequire);

	this->executeMain();

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

// ===================
// === Private API ===
// ===================
void BackendManager::executeMain() {
	// Open the main file and setup the current director.
	std::ifstream mainFile("scripts/main.luau");
	std::filesystem::path currentDir = std::filesystem::current_path();

	// Check if the file is open.
	if (!mainFile.is_open()) {
		std::cerr << "[BackendManager] - An error occured while opening main file.\n";
		return;
	}

	// Check if the stream is good.
	if (mainFile.fail()) {
		std::cerr << "[BackendManager] - Stream is not good.\n";
		return;
	}
	
	// Convert the contents into a string.
	std::string fileContents{
		std::istreambuf_iterator<char>(mainFile),
		std::istreambuf_iterator<char>()
	};

	// Get the absolute path to the main file.
	std::filesystem::path absolutePath = std::filesystem::absolute("scripts/main.luau");

	// Get the bytecode than execute.
	std::vector<char> bytecode = compiler->compile(fileContents.c_str());
	runtime->executeBytecode(bytecode, absolutePath.string().c_str());
}