#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>

#include "../audio/tgAudioSystem.h"
#include "../ecs/tgComponentManager.h"
#include "../graphics/tgRenderer.h"
#include "tgGameState.h"
#include "tgWindow.h"
#include "tgTime.h"

#include <unordered_map>
#include <functional>

class tgEngine {
public:
	tgEngine(tgWindow *window);

	void start();
	void stop();

	tgRenderer* getRenderer() { return m_renderer; }
	tgWindow* getWindow() { return m_window; }

	void setRenderer(tgRenderer *renderer) { m_renderer = renderer; }
	
	void registerState(std::string const& name, tgGameState *state);
	void setState(std::string const& name);
	void reloadState(std::string const& name);

	void setAssetPreloadFunction(std::function<void(void)> const& fun) { m_assetPreLoadFunction = fun; }

private:
	std::function<void(void)> m_assetPreLoadFunction;
	std::unordered_map<std::string, tgGameState*> m_states;
	std::string m_currentState, m_nextState;
	bool m_changingStates;

	tgWindow *m_window;
	tgRenderer *m_renderer;

	bool m_running;

	void mainloop();
};

#endif
