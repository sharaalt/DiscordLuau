#include <iostream>
#include <functional>

struct CallbackType {
	std::function<void(int)> callback;
	int callbackId;
};