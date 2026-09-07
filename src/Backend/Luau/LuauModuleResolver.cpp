#include "LuauModuleResolver.hpp"

std::filesystem::path LuauModuleResolver::resolve(const std::string& moduleName, const std::string& path) {
	std::filesystem::path currentPath(path);
	std::filesystem::path modulePath(moduleName);

	std::filesystem::path resolvedPath;

	// If the module is relative we combine the directories.
	if (isRelative(moduleName)) {
		resolvedPath = currentPath.parent_path() / modulePath;
	}
	else {
		resolvedPath = "scripts" / modulePath;
	}

	// If there is no extension attach a new one.
	if (resolvedPath.extension() != "luau") {
		addExtension(resolvedPath);
	}

	// Normalize the resolved path and update the path string.
	resolvedPath = resolvedPath.lexically_normal();
	return resolvedPath;
};

bool LuauModuleResolver::isRelative(const std::string& moduleName) {
	if (moduleName.starts_with("./")) {
		return true;
	}

	if (moduleName.starts_with("../")) {
		return true;
	}

	return false;
}

void LuauModuleResolver::addExtension(std::filesystem::path& modulename) {
	modulename += ".luau";
}