#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "tgComponent.h"

#include <cstddef>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#define TG_INVALID_ENTITY 0xDEAD
#define TG_INITIAL_ID 0xBEEF

using tgEntity = std::size_t;

class tgComponentManager {
public:
	tgComponentManager();
	~tgComponentManager();

	std::size_t createEntity();
	void destroyEntity(std::size_t entity);

	template <typename T>
	T* addComponent(std::size_t entity) {
		auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
		if (pos != m_entities.end()) {
			T* comp = new T();
			comp->m_owner = entity;
			comp->m_manager = this;
			m_components.push_back(comp);

			return comp;
		}
		return nullptr;
	}

	template <typename T, typename... Args>
	T* addComponent(std::size_t entity, Args&&... args) {
		auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
		if (pos != m_entities.end()) {
			T* comp = new T(std::forward<Args>(args)...);
			comp->m_owner = entity;
			comp->m_manager = this;
			m_components.push_back(comp);

			return comp;
		}
		return nullptr;
	}

	template <typename T>
	T* getComponent(std::size_t entity) {
		for (tgComponent *comp : m_components) {
			if (comp->m_owner == entity) {
				T* t_comp = dynamic_cast<T*>(comp);
				if (t_comp) { return t_comp; }
			}
		}
		return nullptr;
	}

	template <typename T>
	bool contains(std::size_t entity) {
		return getComponent<T>(entity) != nullptr;
	}

	template <typename T, typename V, typename... C>
	bool contains(std::size_t entity) {
		return contains<T>(entity) && contains<V, C...>(entity);
	}

	template <typename... Types>
	std::vector<std::size_t> search() {
		std::vector<std::size_t> ents;
		for (std::size_t e : m_entities) {
			if (contains<Types...>(e)) {
				ents.push_back(e);
			}
		}
		return ents;
	}

	void addComponent(std::size_t entity, tgComponent *comp);
	std::vector<tgComponent*> getComponents(std::size_t entity);

	void start();
	void update(float dt);
	void render();

private:
	std::vector<std::size_t> m_entities;
	std::vector<tgComponent*> m_components;
	std::size_t m_lastEntity;

	void cleanup();
};

#endif
