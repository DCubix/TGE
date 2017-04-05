#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include "sol.hpp"

class tgScriptEngine {
public:
	tgScriptEngine();
	~tgScriptEngine();

private:
	sol::state L;
};

#endif
