#include "tgEngine.h"

#include "tgUtil.h"
#include "tgLog.h"
#include "tgInput.h"
#include "../tween/tgTimer.h"
#include "../tween/tgTweens.h"
#include "../assets/tgAssets.h"

tgEngine::tgEngine(tgWindow *window) {
	m_running = false;
	m_window = window;
}

void tgEngine::start() {
	m_running = true;

	mainloop();

	for (auto &kv : m_states) {
		kv.second->destroy();
		tgDelete(kv.second);
	}

	tgAssets::destroy();

	tgDelete(m_renderer);
	tgDelete(m_window);
}

void tgEngine::stop() {
	m_running = false;
}

void tgEngine::registerState(std::string const& name, tgGameState* state) {
	if (state) {
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
		m_states[m_currentState]->reset(this);
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
	
	tgAssets::create();
	if (m_assetPreLoadFunction) {
		m_assetPreLoadFunction();
	}
	tgAssets::load();

	m_window->show();

	if (m_nextState.empty()) {
		setState(m_states.begin()->first);
	}

	tgGameState *state = m_states[m_nextState];

	while (m_running) {
		if (m_changingStates) {
			state = m_states[m_nextState];
			state->reset(this);
			state->getManager()->start();
			m_changingStates = false;
		}

		currentTime = tgTime::getTime();
		delta = currentTime - startTime;
		startTime = currentTime;

		timeAccum += delta;

		while (timeAccum >= timeDelta) {
			tgInput::update();
			
			if (!m_changingStates) {
				state->getManager()->update(timeDelta);
				state->update(timeDelta);
			}

			tgTimer::update(timeDelta);
			tgTweens::update(timeDelta);

			timeAccum -= timeDelta;
		}

		m_renderer->clear(GL_COLOR_BUFFER_BIT);

		m_renderer->begin();
		if (!m_changingStates) {
			state->getManager()->render(m_renderer);
			state->render(m_renderer);
		}
		m_renderer->end();

		m_renderer->render();

		m_window->swapBuffers();
	}
}
