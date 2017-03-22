#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <functional>

#include "tgEntity.h"
#include "tgSystem.h"

class tgEntitySystemManager {
public:
	tgEntitySystemManager();
	~tgEntitySystemManager();

	tgEntity* create();
	void destroy(tgEntity *entity, bool enqueue=true);

	void addSystem(tgSystem *system);
	void removeSystem(tgSystem *system);

	bool clean();
	void reset();

	template <typename... Args>
	std::vector<tgEntity*> search() {
		std::vector<tgEntity*> ents;
		for (tgEntity *en : m_entities) {
			if (en->contains<Args...>()) {
				ents.push_back(en);
			}
		}
		return ents;
	}
	
	tgEntity* getByID(tgEntityID id);

	std::vector<tgEntity*> getEntities() const { return m_entities; }
	std::vector<tgSystem*> getSystems() const { return m_systems; }

	void render();
	void update(float dt);
	void fixedUpdate(float dt);

private:
	std::vector<tgEntity*> m_entities;
	std::vector<tgSystem*> m_systems;
	tgEntityID m_lastEntityID;
};

#endif
