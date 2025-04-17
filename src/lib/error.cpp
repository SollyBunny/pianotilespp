#include "error.hpp"

#include <stdexcept>

#include <mutex>

#include <cstdarg>
#include <cstdio>

static std::mutex errorLock;

[[gnu::format(printf, 1, 2)]] [[noreturn]] void raiseException(const char* format, ...) {
	std::scoped_lock lock(errorLock);
	static char buffer[2048];
	std::va_list args;
	va_start(args, format);
	std::snprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	throw std::runtime_error(buffer);
}
