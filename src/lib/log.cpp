#include "log.hpp"

#include <filesystem>

#include <cstring>
#include <cstdarg>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static const std::filesystem::path startPath = std::filesystem::path(std::source_location::current().file_name())
	.parent_path().parent_path();
static const std::string getRelativeSourcePath(const std::source_location& location) {
	try {
		return std::filesystem::relative(location.file_name(), startPath);
	} catch (const std::exception& e) {
		std::string out;
		out += location.file_name();
		out += " (";
		out += e.what();
		out += ")";
		return out;
	}
}

[[gnu::always_inline]] static inline bool ansiiEscapesEnabled() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
		return false;
	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
		return false;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	return SetConsoleMode(hOut, dwMode);
#else
	return isatty(fileno(stdout));
#endif
}

bool ansiiEscapes = ansiiEscapesEnabled();
LogLevel logLevel = LogLevel::DEBUG;

static constexpr const char* LOG_LEVEL_LABELS[] = {
	"debug",
	"info ",
	"warn ",
	"error"
};
static constexpr const int LOG_LEVEL_COLOR[] = {
	35, // magenta
	36, // cyan
	33, // yellow
	31, // red
};
template<std::size_t N>
[[gnu::always_inline]] static inline void appendString(const char (&buffer)[N], char*& p, const char* format, std::va_list ap) {
	const char* bufferEnd = buffer + N;
	if (p >= bufferEnd)
		return;
	p += std::vsnprintf(p, N - (p - buffer), format, ap);
}
template<std::size_t N>
[[gnu::format(printf, 3, 4)]] [[gnu::always_inline]] static inline void appendString(const char (&buffer)[N], char*& p, const char* format, ...) {
	std::va_list args;
	va_start(args, format);
	appendString(buffer, p, format, args);
	va_end(args);
}
static thread_local char buffer[4096];
void logImpl(std::source_location location, LogLevel level, const char* format, ...) {
	if ((int)level < (int)logLevel)
		return;
	char* p = buffer;
	if(ansiiEscapes)
		appendString(buffer, p, "\x1b[%dm", LOG_LEVEL_COLOR[(int)level]);
	appendString(buffer, p, "[%s %s:%d] ", LOG_LEVEL_LABELS[(int)level], getRelativeSourcePath(location).c_str(), (int)location.line());
	if(ansiiEscapes)
		appendString(buffer, p, "\x1b[0m");
	std::va_list args;
	va_start(args, format);
	appendString(buffer, p, format, args);
	va_end(args);
	std::puts(buffer);
#undef APPEND
}
