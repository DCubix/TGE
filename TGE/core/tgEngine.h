#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>

#include "../graphics/tgRenderer.h"

#include "tgGameState.h"
#include "tgWindow.h"
#include "tgTime.h"

#include "tgParticleEngine.h"

#include "../audio/tgAudioSystem.h"

#include <unordered_map>
#include <functional>

class tgEngine {
public:
	tgEngine(tgWindow *window);

	void start();
	void stop();

	float getTimeScale() const { return m_timeScale; }
	void setTimeScale(float v) { m_timeScale = tgMathUtil::clamp(v, 0.01f, 1.0f); }

	tgRenderer* getRenderer() { return m_renderer; }
	tgWindow* getWindow() { return m_window; }
	tgAudioSystem* getAudioSystem() { return m_audioSystem; }
	tgParticleEngine* getParticleEngine() { return m_particleEngine; }

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

	tgAudioSystem *m_audioSystem;
	tgParticleEngine *m_particleEngine;

	float m_timeScale;

	bool m_running;

	void mainloop();
};

#endif
