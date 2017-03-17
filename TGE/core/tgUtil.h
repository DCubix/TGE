#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <memory>

unsigned int checkError();
bool glLog(const char* function, const char* file, int line);

#ifndef TG_DISABLE_ERROR_CHECK
#define GLCall(x) checkError(); \
	x; \
	glLog(#x, __FILE__, __LINE__)
#else
#define GLCall(x) x
#endif

#define Log(x) std::cout << x << std::endl
#define Err(x) std::cerr << "Error(" << __FILE__ << ":" << __LINE__ << "):\n\t" << x << std::endl

#define tgDelete(x) if (x != nullptr) { delete x; x = nullptr; }

#endif
