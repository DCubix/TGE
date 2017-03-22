#ifndef SPRITE_RENDERING_SYSTEM_H
#define SPRITE_RENDERING_SYSTEM_H

#include "../graphics/tgSpriteBatch.h"

#include "../ecs/tgSystem.h"

#include <vector>

class tgSpriteRenderingSystem : public tgSystem {
public:
	tgSpriteRenderingSystem(tgSpriteBatch *spriteBatch);
	~tgSpriteRenderingSystem() override final;

	void render(tgEntitySystemManager *mgr) override final;
	
	tgSpriteBatch *getSpriteBatch() const { return m_spriteBatch; }

protected:
	tgSpriteBatch *m_spriteBatch;
};

#endif
