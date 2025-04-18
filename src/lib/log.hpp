#pragma once

#include <source_location>

enum class LogLevel {
	DEBUG = 0,
	INFO = 1,
	WARN = 2,
	ERROR = 3,
};

extern LogLevel logLevel;
extern bool ansiiEscapes;

[[gnu::format(printf, 3, 4)]] void logImpl(std::source_location location, LogLevel level, const char* format, ...);

#define debug(...) logImpl(std::source_location::current(), LogLevel::DEBUG, __VA_ARGS__)
#define info(...)  logImpl(std::source_location::current(), LogLevel::INFO,  __VA_ARGS__)
#define warn(...)  logImpl(std::source_location::current(), LogLevel::WARN,  __VA_ARGS__)
#define error(...) logImpl(std::source_location::current(), LogLevel::ERROR, __VA_ARGS__)
