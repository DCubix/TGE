#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H

#include "../ecs/tgSystem.h"

class tgSpriteSystem : public tgSystem {
public:
	void update(float dt) override;
	void render(tgRenderer *renderer) override;
};

#endif
