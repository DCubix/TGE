#include "tgECS.h"

#include "../core/tgUtil.h"

tgECS::tgECS()
	: m_lastEntityID(ECS_INITIAL_ID)
{
	m_messenger = new tgMessenger();
}

tgECS::~tgECS() {
	for (tgComponent *comp : m_components) {
		tgDelete(comp);
	}
	for (tgEntity *ent : m_entities) {
		tgDelete(ent);
	}
	for (tgSystem *sys : m_systems) {
		tgDelete(sys);
	}
	m_entities.clear();
	m_components.clear();
	m_systems.clear();
}

tgEntity* tgECS::create() {
	tgEntity *ent = new tgEntity();
	ent->m_id = m_lastEntityID++;
	ent->m_ecs = this;
	m_entities.push_back(ent);
	return m_entities.back();
}

void tgECS::destroy(tgEntity* entity) {
	auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
	if (pos != m_entities.end()) {
		uint id = pos - m_entities.begin();
		m_entities[id]->m_id = ECS_INVALID_ENTITY;
		for (uint comp : m_entities[id]->m_components) {
			m_components[comp]->id = ECS_INVALID_COMPONENT;
		}
	}
}

void tgECS::registerSystem(tgSystem *system) {
	if (system) {
		m_systems.push_back(system);
		m_systems.back()->m_ecs = this;
	}
}

std::vector<tgComponent*> tgECS::getComponents(tgEntity* entity) {
	std::vector<tgComponent*> ret;
	if (validEntity(entity)) {
		for (uint comp : entity->m_components) {
			ret.push_back(m_components[comp]);
		}
	}
	return ret;
}

void tgECS::start() {
	for (tgSystem *sys : m_systems) {
		sys->start();
	}
}

void tgECS::update(float dt) {
	cleanup();

	m_messenger->processQueue(this);

	for (tgSystem *sys : m_systems) {
		sys->update(dt);
	}
}

void tgECS::render(tgRenderer *renderer) {
	for (tgSystem *sys : m_systems) {
		sys->render(renderer);
	}
}

void tgECS::reset() {
	for (tgComponent *comp : m_components) {
		tgDelete(comp);
	}
	for (tgEntity *ent : m_entities) {
		tgDelete(ent);
	}
	m_components.clear();
	m_entities.clear();
	m_messenger->reset();
	m_lastEntityID = ECS_INITIAL_ID;

	for (tgSystem *sys : m_systems) {
		sys->reset();
		sys->start();
	}
}

void tgECS::cleanup() {
	std::vector<tgComponent*> rem_components = getInvalidComponents();
	std::vector<uint> rem_entities;

	uint i = 0;
	for (tgEntity *ent : m_entities) {
		if (ent->m_id == ECS_INVALID_ENTITY) {
			rem_entities.push_back(i);
		}
		i++;
	}

	for (tgComponent *comp : rem_components) {
		m_components.erase(std::remove(m_components.begin(), m_components.end(), comp), m_components.end());
		tgDelete(comp);
	}
	for (uint i : rem_entities) {
		tgEntity *ent = m_entities[i];
		m_entities[i] = m_entities.back();
		m_entities.pop_back();
		tgDelete(ent);
	}
}

bool tgECS::valid(uint id) {
	auto pos = std::find_if(m_entities.begin(), m_entities.end(), [&](tgEntity* e) {
		return e->getID() == id;
	});
	return pos != m_entities.end();
}

std::vector<tgComponent*> tgECS::getInvalidComponents() {
	std::vector<tgComponent*> ret;
	std::copy_if(m_components.begin(), m_components.end(), ret.begin(), [&, this](tgComponent* c) {
		return c->getID() == ECS_INVALID_COMPONENT;
	});
	return ret;
}
