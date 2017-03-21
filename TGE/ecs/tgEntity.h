#ifndef ENTITY_H
#define ENTITY_H

#include <cstddef>
#include <vector>
#include <algorithm>

#include "tgComponentHandler.h"
#include "tgComponent.h"

using tgEntityID = std::size_t;

class tgWorld;
class tgEntity {
	friend class tgWorld;
public:
	virtual ~tgEntity();

	template <typename T>
	bool contains() const {
		for(tgComponent *comp : m_components) {
			return dynamic_cast<T*>(comp) != nullptr;
		}
	}

	template <typename T, typename V, typename... Types>
	bool contains() const {
		return contains<T>() && contains<V, Types...>();
	}

	template <typename T>
	tgComponentHandler<T> add() {
		m_components.push_back(new T());
		return tgComponentHandler<T>(dynamic_cast<T*>(m_components.back()));
	}

	template <typename T, typename... Args>
	tgComponentHandler<T> add(Args&&... args) {
		m_components.push_back(new T(args...));
		return tgComponentHandler<T>(dynamic_cast<T*>(m_components.back()));
	}

	template <typename T>
	tgComponentHandler<T> get() {
		for(tgComponent *comp : m_components) {
			T* t_comp = dynamic_cast<T*>(comp);
			if(t_comp != nullptr) {
				return tgComponentHandler<T>(t_comp);
			}
		}
		return nullptr;
	}

	template <typename T, typename V, typename... Types>
	std::vector<tgComponentHandler<T>> getAll() {
		std::vector<tgComponentHandler<T>> comps;
		comps.push_back(get<T>());
		comps.push_back(get<V, Types...>());
		return comps;
	}

	std::vector<tgComponent*> getComponents() const { return m_components; }

	template <typename T>
	void remove() {
		T* comp = _get<T>();
		if(comp) {
			m_components.erase(
				std::remove(m_components.begin(), m_components.end(), comp), m_components.end()
			);
		}
	}

	void removeAll();

	tgEntityID getID() const { return m_id; }
	tgWorld *getWorld() const { return m_world; }
	bool isDead() const { return m_dead; }

protected:
	tgEntity(tgWorld *world, tgEntityID id);

	template <typename T>
	T* _get() const {
		for(tgComponent *comp : m_components) {
			T* t_comp = dynamic_cast<T*>(comp);
			if(t_comp != nullptr) {
				return t_comp;
			}
		}
		return nullptr;
	}

	tgEntityID m_id;
	tgWorld *m_world;
	bool m_dead;
	std::vector<tgComponent*> m_components;
};

#endif
