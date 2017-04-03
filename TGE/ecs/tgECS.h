#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "tgComponent.h"
#include "tgSystem.h"

#include <cstddef>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#define ECS_INVALID_ENTITY -1

class tgECS;
class tgEntity {
	friend class tgECS;
public:

	template <typename T> T* add();
	template <typename T, typename... Args> T* add(Args&&... args);
	template <typename T> T* get();
	template <typename... Ts> bool has();

	void destroy();

	uint getID() const { return m_id; }

protected:
	tgEntity() {}

	uint m_id;
	std::vector<uint> m_components;
	tgECS *m_ecs;
};

class tgRenderer;
class tgECS {
public:
	tgECS();
	~tgECS();

	tgEntity* create();
	void destroy(tgEntity* entity);

	void registerSystem(tgSystem *system);

	template <typename T>
	T* addComponent(tgEntity* entity) {
		return addComponent<T>(entity, new T());
	}

	template <typename T, typename... Args>
	T* addComponent(tgEntity* entity, Args&&... args) {
		return addComponent<T>(entity, new T(std::forward<Args>(args)...));
	}

	template <typename T>
	T* getComponent(tgEntity* entity) {
		if (validEntity(entity)) {
			for (tgComponent *comp : m_components) {
				if (comp->owner == entity->m_id) {
					T* t_comp = dynamic_cast<T*>(comp);
					if (t_comp) { return t_comp; }
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	bool hasComponent(tgEntity* entity) {
		return getComponent<T>(entity) != nullptr;
	}

	template <typename T, typename V, typename... C>
	bool hasComponent(tgEntity* entity) {
		return hasComponent<T>(entity) && hasComponent<V, C...>(entity);
	}

	template <typename... Ts>
	std::vector<tgEntity*> with() {
		std::vector<tgEntity*> ret;
		for (tgEntity *ent : m_entities) {
			if (hasComponent<Ts...>(ent)) {
				ret.push_back(ent);
			}
		}
		return ret;
	}

	std::vector<tgComponent*> getComponents(tgEntity* entity);

	std::vector<tgEntity*> getEntities() const { return m_entities; }
	std::vector<tgComponent*> getComponents() const { return m_components; }
	std::vector<tgSystem*> getSystems() const { return m_systems; }

	void start();
	void update(float dt);
	void render(tgRenderer *renderer);

	void reset();

	bool valid(uint id);

private:
	std::vector<tgEntity*> m_entities;
	std::vector<tgComponent*> m_components;
	std::vector<tgSystem*> m_systems;
	uint m_lastEntityID;

	void cleanup();

	bool validEntity(tgEntity* entity) { return valid(entity->m_id); }

	std::vector<tgComponent*> getInvalidComponents();

	template <typename T>
	T* addComponent(tgEntity* entity, T *comp) {
		if (validEntity(entity)) {
			m_components.push_back(comp);

			comp->owner = entity->m_id;
			entity->m_components.push_back(m_components.size() - 1);

			return dynamic_cast<T*>(m_components.back());
		}
		return nullptr;
	}
};

template <typename T> T* tgEntity::add() {
	return m_ecs->addComponent<T>(this);
}

template<typename T, typename ...Args>
T* tgEntity::add(Args&&... args) {
	return m_ecs->addComponent<T>(this, std::forward<Args>(args)...);
}

template<typename T>
T* tgEntity::get() {
	return m_ecs->getComponent<T>(this);

}

template<typename ...Ts>
bool tgEntity::has() {
	return m_ecs->hasComponent<Ts...>(this);
}

#endif
