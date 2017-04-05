#include "tgEngine.h"

#include "tgUtil.h"
#include "tgLog.h"
#include "tgInput.h"
#include "../tween/tgTimer.h"
#include "../tween/tgTweens.h"
#include "../assets/tgAssets.h"

tgEngine::tgEngine(tgWindow *window) : m_timeScale(1.0f){
	m_running = false;
	m_window = window;
}

void tgEngine::start() {
	m_running = true;

	tgAssets::create();

	m_audioSystem = new tgAudioSystem();
	m_particleEngine = new tgParticleEngine(100000);

	mainloop();

	for (auto &kv : m_states) {
		kv.second->destroy();
		tgDelete(kv.second);
	}

	tgAssets::destroy();

	tgDelete(m_audioSystem);
	tgDelete(m_particleEngine);
	tgDelete(m_renderer);
	tgDelete(m_window);
}

void tgEngine::stop() {
	m_running = false;
}

void tgEngine::registerState(std::string const& name, tgGameState* state) {
	if (state) {
		state->m_engine = this;
		m_states[name] = state;
	}
}

void tgEngine::setState(std::string const& name) {
	m_nextState = name;
	if (m_nextState != m_currentState) {
		m_changingStates = true;
		m_currentState = m_nextState;
	}
}

void tgEngine::reloadState(std::string const& name) {
	if (!m_currentState.empty()) {
		m_states[m_currentState]->destroy();
		m_states[m_currentState]->getECS()->reset();
		m_states[m_currentState]->start();
	}
}

void tgEngine::mainloop() {
	if (m_states.empty()) {
		tgLog::log("No game states have been registered. Exiting.");
		return;
	}

	const float timeDelta = 1.0f / 60.0f;
	float timeAccum = 0.0f;
	float startTime = tgTime::getTime();
	float currentTime, delta;
	
	tgAssets::addSource("."); // Adds current directory to the sources list.

	m_window->show();

	if (m_nextState.empty()) {
		setState(m_states.begin()->first);
	}

	tgGameState *state = m_states[m_nextState];

	while (m_running) {
		if (m_changingStates) {
			state = m_states[m_nextState];
			m_particleEngine->restart();
			state->getECS()->reset();
			state->start();
			state->getECS()->start();
			m_changingStates = false;
		}

		currentTime = tgTime::getTime();
		delta = currentTime - startTime;
		startTime = currentTime;

		timeAccum += delta;

		while (timeAccum >= timeDelta) {
			float dt = m_timeScale * timeDelta;

			tgInput::update();
			if (tgInput::isCloseRequested()) {
				stop();
			}
			
			if (!m_changingStates) {
				state->getECS()->update(dt);
				state->update(dt);
				m_particleEngine->update(dt);
				m_audioSystem->update();
			}

			tgTimer::update(dt);
			tgTweens::update(dt);

			timeAccum -= timeDelta;
		}

		m_renderer->clear(GL_COLOR_BUFFER_BIT);

		m_renderer->begin();
		if (!m_changingStates) {
			state->getECS()->render(m_renderer);

			tgSpriteBatch *sb = dynamic_cast<tgSpriteBatch*>(m_renderer);
			if (sb)
				m_particleEngine->render(sb);

			state->render(m_renderer);
		}
		m_renderer->end();
		m_renderer->render();

		m_window->swapBuffers();
	}
}
