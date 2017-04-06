#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>

#include "../graphics/tgRenderer.h"

#include "tgGameState.h"
#include "tgWindow.h"
#include "tgTime.h"
#include "tgParticleEngine.h"

#include "../audio/tgAudioSystem.h"

#include "../ecs/tgECS.h"

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
	tgECS* getECS() { return m_ecs; }

	tgVector4& getBackColor() { return m_backColor; }
	void setBackColor(tgVector4 const& v) { m_backColor = v; }

	void registerState(std::string const& name, tgGameState *state);
	void setState(std::string const& name);
	void reloadState(std::string const& name);

private:
	std::unordered_map<std::string, tgGameState*> m_states;
	std::string m_currentState, m_nextState;
	bool m_changingStates;

	tgVector4 m_backColor;

	tgWindow *m_window;
	tgRenderer *m_renderer;
	tgAudioSystem *m_audioSystem;
	tgParticleEngine *m_particleEngine;
	tgECS *m_ecs;

	float m_timeScale;

	bool m_running;

	void mainloop();
};

#endif
