#include "tgEntity.h"

#include "../core/tgUtil.h"
#include "tgEntitySystemManager.h"

tgEntity::tgEntity(tgEntitySystemManager *m, tgEntityID id) : m_id(id), m_manager(m), m_dead(false) {}

tgEntity::~tgEntity() {
	removeAll();
}

inline void tgEntity::removeAll() {
	for(tgComponent *comp : m_components) {
		tgDelete(comp);
	}
	m_components.clear();
}

void tgEntity::kill(bool enqueue) {
	m_manager->destroy(this, enqueue);
}
