#ifndef SCRIPT_H
#define SCRIPT_H

#include <functional>

class tgEntitySystemManager;
class tgScript {
public:
	tgScript();
	
private:
	std::function<void(tgEntitySystemManager*, float)> m_updateFunction;
};

#endif
