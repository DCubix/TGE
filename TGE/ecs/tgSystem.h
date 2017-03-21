#ifndef SYSTEM_H
#define SYSTEM_H

class tgWorld;
class tgSystem {
public:
	virtual ~tgSystem() = default;
	virtual void update(tgWorld *world, float dt) = 0;
};

#endif
