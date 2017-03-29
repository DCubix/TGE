#include "tgGameState.h"

tgGameState::tgGameState() {
	m_manager = new tgComponentManager();
}

tgGameState::~tgGameState() {
	tgDelete(m_manager);
}

void tgGameState::reset(tgEngine* engine) {
	destroy();
	m_manager->reset();
	start(engine);
}
