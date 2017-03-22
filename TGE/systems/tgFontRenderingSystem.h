#ifndef FONT_RENDERING_SYSTEM_H
#define FONT_RENDERING_SYSTEM_H

#include "../graphics/tgSpriteBatch.h"
#include "../ecs/tgSystem.h"

class tgFontRenderingSystem : public tgSystem {
public:
	tgFontRenderingSystem(tgSpriteBatch *spriteBatch);
	~tgFontRenderingSystem() override final;

	void render(tgEntitySystemManager *mgr) override final;

	tgSpriteBatch *getSpriteBatch() const { return m_spriteBatch; }

private:
	tgSpriteBatch *m_spriteBatch;
};

#endif
