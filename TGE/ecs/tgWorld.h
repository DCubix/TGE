#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <functional>

#include "tgEntity.h"
#include "tgSystem.h"

class tgWorld {
public:
	tgWorld();
	~tgWorld();

	tgEntity* create();
	void destroy(tgEntity *entity, bool enqueue=true);

	void addSystem(tgSystem *system);
	void removeSystem(tgSystem *system);

	bool clean();
	void reset();

	template <typename... Types>
	void forEach(std::function<void(tgEntity*, tgComponentHandler<Types>...)> fun, bool includeTheDead=false) {
		for(tgEntity *ent : m_entities) {
			if(includeTheDead) {
				if(ent->contains<Types...>()) {
					fun(ent, ent->get<Types>()...);
				}
			} else {
				if(ent->contains<Types...>() && !ent->isDead()) {
					fun(ent, ent->get<Types>()...);
				}
			}
		}
	}

	tgEntity* getByID(tgEntityID id);

	std::vector<tgEntity*> getEntities() const { return m_entities; }
	std::vector<tgSystem*> getSystems() const { return m_systems; }

	void update(float dt);

private:
	std::vector<tgEntity*> m_entities;
	std::vector<tgSystem*> m_systems;
	tgEntityID m_lastEntityID;
};

#endif
