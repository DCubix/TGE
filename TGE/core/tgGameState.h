#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../ecs/tgComponentManager.h"
#include "tgUtil.h"

class tgEngine;
class tgRenderer;
class tgGameState {
public:
	tgGameState();
	~tgGameState();

	virtual void start(tgEngine* engine) {}
	virtual void update(float dt) {}
	virtual void render(tgRenderer*) {}
	virtual void destroy() {}

	void reset(tgEngine* engine);

	tgComponentManager* getManager() { return m_manager; }
protected:
	tgComponentManager *m_manager;
};

#endif