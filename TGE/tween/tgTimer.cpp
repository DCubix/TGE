#include "tgTimer.h"

#include "../core/tgUtil.h"

std::queue<tgTimeEvent> tgTimer::m_timers;
tgTimeEvent tgTimer::m_currentTimer;

void tgTimer::wait(float secs, std::function<void(void)> const& onComplete) {
	tgTimeEvent e;
	e.onComplete = onComplete;
	e.time = secs;
	m_timers.push(e);
}

void tgTimer::update(float dt) {
	if (!m_timers.empty()) {
		if (m_currentTimer.time <= -1) {
			m_currentTimer = m_timers.front();
		} else {
			m_currentTimer.time -= dt;
			if (m_currentTimer.time <= 0) {
				if (m_currentTimer.onComplete) {
					m_currentTimer.onComplete();
				}
				m_timers.pop();
				m_currentTimer.time = -1;
			}
		}
	}
}
