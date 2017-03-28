#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>

#include "../audio/tgAudioSystem.h"
#include "../ecs/tgComponentManager.h"
#include "../graphics/tgRenderer.h"
#include "tgGame.h"
#include "tgWindow.h"
#include "tgTime.h"

class tgEngine {
public:
	tgEngine(tgWindow *window);

	void start();
	void stop();

	tgAudioSystem* getAudioSystem() { return m_audioSystem; }
	tgComponentManager* getEntityComponentManager() { return m_entityComponentManager; }
	tgRenderer* getRenderer() { return m_renderer; }
	tgWindow* getWindow() { return m_window; }

	void setRenderer(tgRenderer *renderer) { m_renderer = renderer; }

	void setGame(tgGame *game) { m_game = game; }

private:
	tgGame *m_game;
	tgWindow *m_window;

	tgAudioSystem *m_audioSystem;
	tgComponentManager *m_entityComponentManager;
	tgRenderer *m_renderer;

	bool m_running;

	void mainloop();
};

#endif
