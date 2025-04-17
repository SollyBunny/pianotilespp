#pragma once

[[gnu::format(printf, 1, 2)]] [[noreturn]] void raiseException(const char* format, ...);
