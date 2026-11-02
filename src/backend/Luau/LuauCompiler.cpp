#include <luau/LuauCompiler.hpp>

std::vector<char> LuauCompiler::compile(const char* source) {
	// Compile the source into bytecode.
	size_t bytecodeSize = 0;
	char* byteCode = luau_compile(source, strlen(source), NULL, &bytecodeSize);

	// Create a new bytecode buffer and free up the byte code.
	std::vector<char> bytecodeBuffer;
	bytecodeBuffer.assign(byteCode, byteCode + bytecodeSize);
	free(byteCode);

	// Print success and return the bytecode.
	std::cout << "[LuauCompiler] - Sucessfully compiled into bytecode." << '\n';
	return bytecodeBuffer;
}