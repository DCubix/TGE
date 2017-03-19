#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class tgLog {
public:
	
	template <typename... Args>
	static void log(Args... args) {
		std::stringstream stm;
		int dummy[sizeof...(Args)] = { (stm << args, 0)... };
		stm << std::endl;

		// Prevent from flooding the console.
		if(stm.str() != m_prevLog) {
			std::cout << stm.str();
		}
		m_prevLog = stm.str();
	}

	template <typename... Args>
	static void print(Args... args) {
		int dummy[sizeof...(Args)] = { (std::cout << args, 0)... };
	}

	template <typename... Args>
	static void println(Args... args) {
		int dummy[sizeof...(Args)] = { (std::cout << args, 0)... };
		std::cout << std::endl;
	}

private:
	static std::string m_prevLog;
};

#endif