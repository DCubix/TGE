#include "tgEngine.h"

#include "tgUtil.h"
#include "tgInput.h"
#include "../tween/tgTimer.h"
#include "../tween/tgTweens.h"

tgEngine::tgEngine(tgWindow *window) {
	m_running = false;
	m_window = window;
	m_audioSystem = new tgAudioSystem();
	m_entityComponentManager = new tgComponentManager(this);
}

void tgEngine::start() {
	m_running = true;
	mainloop();

	if (m_game) {
		m_game->exit();
	}
	tgDelete(m_window);
	tgDelete(m_audioSystem);
	tgDelete(m_entityComponentManager);
	tgDelete(m_renderer);
}

void tgEngine::stop() {
	m_running = false;
}

void tgEngine::mainloop() {
	const float timeDelta = 1.0f / 60.0f;
	float timeAccum = 0.0f;
	float startTime = tgTime::getTime();
	float currentTime, delta;

	if (m_game) {
		m_game->start(this);
	}
	m_entityComponentManager->start();

	while (m_running) {
		currentTime = tgTime::getTime();
		delta = currentTime - startTime;
		startTime = currentTime;

		timeAccum += delta;

		m_audioSystem->update();
		while (timeAccum >= timeDelta) {
			tgInput::update();
			
			if (m_game) {
				m_game->update(timeDelta);
			}
			m_entityComponentManager->update(timeDelta);
			
			tgTimer::update(timeDelta);
			tgTweens::update(timeDelta);

			timeAccum -= timeDelta;
		}

		m_renderer->clear(GL_COLOR_BUFFER_BIT);

		m_renderer->begin();
		m_entityComponentManager->render(m_renderer);
		m_renderer->end();

		m_renderer->render();

		m_window->swapBuffers();
	}
}
