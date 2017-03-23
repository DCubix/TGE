#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstddef>

class tgComponentManager;
class tgComponent {
	friend class tgComponentManager;
public:
	tgComponent() : m_enabled(true) {}
	virtual ~tgComponent() = default;

	std::size_t getOwner() const { return m_owner; }
	tgComponentManager *getManager() const { return m_manager; }

	bool isEnabled() const { return m_enabled; }
	void setEnabled(bool enabled) { m_enabled = enabled; }

	virtual void start() {}
	virtual void update(float dt) {}
	virtual void render() {}
	virtual void receive() {} // TODO: Messages
	virtual void collided() {} // TODO: Box2D

protected:
	std::size_t m_owner;
	tgComponentManager *m_manager;
	bool m_enabled;
};

#endif
