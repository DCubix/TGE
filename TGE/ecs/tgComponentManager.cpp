#include "tgComponentManager.h"

#include "../core/tgUtil.h"
#include "../ecs/tgMessenger.h"

tgComponentManager::tgComponentManager(tgEngine *engine)
	: m_lastEntity(TG_INITIAL_ID),
	m_engine(engine)
{
	m_messenger = new tgMessenger();
}

tgComponentManager::~tgComponentManager() {
	for (tgComponent *comp : m_components) {
		tgDelete(comp);
	}
	m_components.clear();
}

std::size_t tgComponentManager::createEntity() {
	m_entities.push_back(m_lastEntity++);
	return m_entities.back();
}

void tgComponentManager::destroyEntity(std::size_t entity) {
	auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
	if (pos != m_entities.end()) {
		m_entities[pos - m_entities.begin()] = TG_INVALID_ENTITY;
	}
}

void tgComponentManager::addComponent(std::size_t entity, tgComponent * comp) {
	auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
	if (pos != m_entities.end()) {
		comp->m_owner = entity;
		comp->m_manager = this;
		m_components.push_back(comp);
	}
}

std::vector<tgComponent*> tgComponentManager::getComponents(std::size_t entity) {
	std::vector<tgComponent*> ret;
	auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
	if (pos != m_entities.end()) {
		for (tgComponent *comp : m_components) {
			if (comp->m_owner == entity) {
				ret.push_back(comp);
			}
		}
	}
	return ret;
}

void tgComponentManager::start() {
	for (tgComponent *comp : m_components) {
		if (comp->isEnabled()) {
			comp->start();
		}
	}
}

void tgComponentManager::update(float dt) {
	cleanup();
	
	m_messenger->processQueue(this);

	for (tgComponent *comp : m_components) {
		if (comp->isEnabled()) {
			comp->update(dt);
		}
	}
}

void tgComponentManager::render(tgRenderer *renderer) {
	for (tgComponent *comp : m_components) {
		if (comp->isEnabled()) {
			comp->render(renderer);
		}
	}
}

void tgComponentManager::cleanup() {
	std::vector<tgComponent*> rem_components;
	std::vector<std::size_t> rem_entities;
	for (std::size_t ent : m_entities) {
		if (ent == TG_INVALID_ENTITY) {
			std::vector<tgComponent*> components = getComponents(ent);
			rem_components.insert(rem_components.end(), components.begin(), components.end());
			rem_entities.push_back(ent);
		}
	}

	if (rem_components.size() > 0) {
		for (tgComponent *comp : rem_components) {
			m_components.erase(std::remove(m_components.begin(), m_components.end(), comp), m_components.end());
			tgDelete(comp);
		}
	}
	if (rem_entities.size() > 0) {
		for (std::size_t ent : rem_entities) {
			m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), ent), m_entities.end());
		}
	}
}
 