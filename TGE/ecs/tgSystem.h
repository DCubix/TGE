#ifndef SYSTEM_H
#define SYSTEM_H

class tgEntitySystemManager;

class tgSystem {
public:
	virtual ~tgSystem() = default;
	virtual void render(tgEntitySystemManager *manager) {}
	virtual void update(tgEntitySystemManager *manager, float dt) {}
};

#endif
