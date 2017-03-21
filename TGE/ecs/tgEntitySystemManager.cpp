#include "tgEntitySystemManager.h"

#include "../core/tgUtil.h"

tgEntitySystemManager::tgEntitySystemManager() : m_lastEntityID(0) {}

tgEntitySystemManager::~tgEntitySystemManager() {
	for(tgEntity *ent : m_entities) {
		tgDelete(ent);
	}
	for(tgSystem *sys : m_systems) {
		tgDelete(sys);
	}
}

tgEntity* tgEntitySystemManager::create() {
	++m_lastEntityID;
	m_entities.push_back(new tgEntity(this, m_lastEntityID));
	return m_entities.back();
}

void tgEntitySystemManager::destroy(tgEntity *entity, bool enqueue) {
	if(entity == nullptr) {
		return;
	}

	if(entity->m_dead) {
		if(!enqueue) {
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
			tgDelete(entity);
		}
		return;
	}

	entity->m_dead = true;

	if(!enqueue) {
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
		tgDelete(entity);
	}
}

void tgEntitySystemManager::addSystem(tgSystem *system) {
	m_systems.push_back(system);
}

void tgEntitySystemManager::removeSystem(tgSystem *system) {
	m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system), m_systems.end());
}

bool tgEntitySystemManager::clean() {
	std::size_t count = 0;
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [&, this](tgEntity *ent) {
		if(ent->isDead()) {
			tgDelete(ent);
			++count;
			return true;
		}
		return false;
	}), m_entities.end());
	return count > 0;
}

void tgEntitySystemManager::reset() {
	for(tgEntity *ent : m_entities) {
		tgDelete(ent);
	}
	m_lastEntityID = 0;
	m_entities.clear();
}

tgEntity* tgEntitySystemManager::getByID(tgEntityID id) {
	for(tgEntity *ent : m_entities) {
		if(ent->getID() == id) {
			return ent;
		}
	}
	return nullptr;
}

void tgEntitySystemManager::render() {
	for (tgSystem *system : m_systems) {
		system->render(this);
	}
}

void tgEntitySystemManager::update(float dt) {
	clean();

	for(tgSystem *system : m_systems) {
		system->update(this, dt);
	}
}
