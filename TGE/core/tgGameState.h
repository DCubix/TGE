#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "tgUtil.h"

class tgECS;
class tgEngine;
class tgRenderer;
class tgGameState {
	friend class tgEngine;
public:
	tgGameState() = default;
	virtual ~tgGameState() = default;

	virtual void start() {}
	virtual void update(float dt) {}
	virtual void render(tgRenderer*) {}
	virtual void destroy() {}
	
	tgECS* getECS();
	tgEngine* getEngine() { return m_engine; }

protected:
	tgEngine *m_engine;
};

#endif