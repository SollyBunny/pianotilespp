#pragma once

#include <type_traits>

[[gnu::format(printf, 1, 2)]] [[noreturn]] void raiseException(const char* format, ...);

template <typename Return>
static auto checkFunctionImpl(const char* (*getError)(), const char* functionName, Return ret) {
	if constexpr (std::is_same_v<Return, bool>) {
		if (ret == true)
			return;
	} else if constexpr (std::is_pointer_v<Return>) {
		if (ret != nullptr)
			return ret;
	} else if constexpr (std::is_arithmetic_v<Return>) {
		if (ret != 0)
			return ret;
	} else {
		static_assert(false, "Can't handle this type");
	}
	const char* e = getError();
	if(e)
		raiseException("%s failed: %s", functionName, e);
	else
		raiseException("%s failed", functionName);
}

[[gnu::always_inline]] const char *strerror_threadsafe();

#define checkFunction(getError, function, ...) checkFunctionImpl(getError, #function, function(__VA_ARGS__))
#define checkFunctionSDL(function, ...) checkFunction(SDL_GetError, function, __VA_ARGS__)
#define checkFunctionErrno(function, ...) checkFunction(strerror_threadsafe, function, __VA_ARGS__)
