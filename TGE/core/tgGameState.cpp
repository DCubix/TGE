#include "tgGameState.h"

tgGameState::tgGameState() {
	m_ecs = new tgECS();
}

tgGameState::~tgGameState() {
	tgDelete(m_ecs);
}

void tgGameState::reset() {
	destroy();
	m_ecs->reset();
	start();
}

tgECS* tgGameState::getECS() {
	if (!m_ecs) {
		m_ecs = new tgECS();
	}
	return m_ecs;
}
