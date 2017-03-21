#include "tgEntity.h"

#include "../core/tgUtil.h"

tgEntity::tgEntity(tgWorld *world, tgEntityID id) : m_id(id), m_world(world), m_dead(false) {}

tgEntity::~tgEntity() {
	removeAll();
}

inline void tgEntity::removeAll() {
	for(tgComponent *comp : m_components) {
		tgDelete(comp);
	}
	m_components.clear();
}
