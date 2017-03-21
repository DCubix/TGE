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

	template <typename T>
	bool search(std::vector<tgEntity*> &vec) {
		for (tgEntity *en : m_entities) {
			if (en->contains<T>()) {
				vec.push_back(en);
				return true;
			}
		}
		return false;
	}

	template <typename T, typename... Args>
	std::vector<tgEntity*> search() {
		std::vector<tgEntity*> ents;
		search< Args...>(ents);
		return ents;
	}
	
	tgEntity* getByID(tgEntityID id);

	std::vector<tgEntity*> getEntities() const { return m_entities; }
	std::vector<tgSystem*> getSystems() const { return m_systems; }

	void render();
	void update(float dt);

private:
	std::vector<tgEntity*> m_entities;
	std::vector<tgSystem*> m_systems;
	tgEntityID m_lastEntityID;
};

#endif
