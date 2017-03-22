#include "tgTimer.h"

#include "../core/tgUtil.h"

std::queue<tgTimeEvent*> tgTimer::m_timers;
tgTimeEvent* tgTimer::m_currentTimer = nullptr;

void tgTimer::wait(float secs, std::function<void(void)> const& onComplete) {
	tgTimeEvent *e = new tgTimeEvent();
	e->onComplete = onComplete;
	e->time = secs;
	m_timers.push(e);
}

void tgTimer::update(float dt) {
	if (!m_timers.empty()) {
		if (m_currentTimer == nullptr) {
			m_currentTimer = m_timers.front();
		} else {
			m_currentTimer->time -= dt;
			if (m_currentTimer->time <= 0) {
				if (m_currentTimer->onComplete) {
					m_currentTimer->onComplete();
				}
				m_timers.pop();
				tgDelete(m_currentTimer);
			}
		}
	}
}
