#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../ecs/tgECS.h"
#include "tgUtil.h"

class tgEngine;
class tgRenderer;
class tgGameState {
	friend class tgEngine;
public:
	tgGameState();
	virtual ~tgGameState();

	virtual void start() {}
	virtual void update(float dt) {}
	virtual void render(tgRenderer*) {}
	virtual void destroy() {}
	
	tgECS* getECS();
	tgEngine* getEngine() { return m_engine; }

protected:
	tgECS *m_ecs;
	tgEngine *m_engine;
};

#endif