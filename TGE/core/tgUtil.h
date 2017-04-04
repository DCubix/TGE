#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

unsigned int checkError();
unsigned int checkALError();
bool glLog(const char* function, const char* file, int line);
void alLog(const char* function, const char* file, int line);

#define GLCall(x) checkError(); \
	x; \
	glLog(#x, __FILE__, __LINE__)

#define ALCall(x) checkALError(); \
	x; \
	alLog(#x, __FILE__, __LINE__);

#define tgDelete(x) if (x != nullptr) { delete x; x = nullptr; }

#endif
