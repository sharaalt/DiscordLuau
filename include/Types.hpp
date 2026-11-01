#define TYPES_HPP
#ifdef TYPES_HPP

#include <iostream>
#include <functional>

#include <nlohmann/json.hpp>

struct CallbackType {
	std::function<void(const nlohmann::json&)> callback;
	int callbackId;
};

#endif // TYPES_HPP