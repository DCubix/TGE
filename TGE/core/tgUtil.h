#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <memory>

unsigned int checkError();
bool glLog(const char* function, const char* file, int line);

#define GLCall(x) checkError(); \
	x; \
	glLog(#x, __FILE__, __LINE__)

#define tgDelete(x) if (x != nullptr) { delete x; x = nullptr; }

#endif
