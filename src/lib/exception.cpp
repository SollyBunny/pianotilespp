#include "exception.hpp"

#include <cstdio>
#include <cstdarg>
#include <cerrno>

#include <stdexcept>

#include <string.h> // For strerror_*

static thread_local char buffer[4096];

void raiseException(const char* format, ...) {
	std::va_list args;
	va_start(args, format);
	std::vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	throw std::runtime_error(buffer);
}

const char *strerror_threadsafe() {
	strerror_r(errno, buffer, sizeof(buffer));
	return buffer;
}
