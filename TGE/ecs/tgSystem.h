#ifndef SYSTEM_H
#define SYSTEM_H

class tgECS;
class tgRenderer;
class tgSystem {
	friend class tgECS;
public:
	tgSystem() = default;
	virtual ~tgSystem() = default;

	virtual void start() {}
	virtual void update(float) {}
	virtual void render(tgRenderer*) {}
	virtual void reset() {}

	tgECS* getECS() { return m_ecs; }

protected:
	tgECS *m_ecs;
};

#endif
