#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include "../ecs/tgSystem.h"

class tgTextSystem : public tgSystem {
public:
	void render(tgRenderer *renderer) override;
};

#endif
