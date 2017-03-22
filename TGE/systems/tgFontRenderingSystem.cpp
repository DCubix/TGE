#include "tgFontRenderingSystem.h"

#include "../core/tgUtil.h"
#include "../ecs/tgEntitySystemManager.h"

#include "../components/tgFontComponent.h"
#include "../components/tgTransformComponent.h"

#include <cctype>

tgFontRenderingSystem::tgFontRenderingSystem(tgSpriteBatch *spriteBatch) {
	m_spriteBatch = spriteBatch;
}

tgFontRenderingSystem::~tgFontRenderingSystem() {
}

static float computeTextWidth(tgFontComponent *fcomp, tgVector2 const& scale) {
	tgFont *fnt = fcomp->getFont();
	float w = 0;
	float padX = fnt->getPadding().x();
	float xscale = scale.x();
	float yscale = scale.y();
	for (int i = 0; i < fcomp->getText().size(); i++) {
		char c = fcomp->getText()[i];
		tgChar chr;
		if (fnt->getCharMap().find(int(c)) != fnt->getCharMap().end()) {
			chr = fnt->getCharMap()[int(c)];
		} else {
			chr = fnt->getCharMap()[fnt->getNumChars() - 1];
		}
		w += ((chr.xadvance - padX) * xscale) + (chr.xoffset * xscale);
	}
	return w;
}

void tgFontRenderingSystem::render(tgEntitySystemManager *mgr) {
	m_spriteBatch->begin();

	for (tgEntity *entity : mgr->search<tgFontComponent, tgTransformComponent>()) {
		tgComponentHandler<tgFontComponent> font = entity->get<tgFontComponent>();
		tgComponentHandler<tgTransformComponent> transform = entity->get<tgTransformComponent>();

		tgFont *fnt = font->getFont();
		tgTexture *tex = fnt->getTexture();
		if (fnt && tex) {
			tgVector2 pos = transform->getTransform()->getWorldPosition().xy();
			tgVector2 scl = transform->getTransform()->getWorldScaling().xy();

			float x = 0, y = 0;
			float xscale = scl.x();
			float yscale = scl.y();
			float padX = fnt->getPadding()[1] + fnt->getPadding()[3];
			float padY = fnt->getPadding()[0] + fnt->getPadding()[2];

			for (int i = 0; i < font->getText().size(); i++) {
				char c = font->getText()[i];
				tgChar chr;
				if (fnt->getCharMap().find(int(c)) != fnt->getCharMap().end()) {
					chr = fnt->getCharMap()[int(c)];
				} else {
					chr = fnt->getCharMap()[fnt->getNumChars() - 1];
				}
				if (!std::isspace(c)) {
					m_spriteBatch->draw(
							fnt->getTexture(),
							chr.clipRect,
							tgVector4(pos.x() + (x + chr.xoffset * xscale), pos.y() + (y + chr.yoffset * yscale),
										float(tex->getWidth()) * xscale, float(tex->getHeight()) * yscale),
							tgVector2(0.0f),
							0,
							tgVector4(1.0f)
					);
				}
				x += (chr.xadvance - padX * 2) * xscale;
			}
			x = 0;
			y += ((fnt->getLineHeight() + 1) - padY * 2) * yscale;
		}
	}

	m_spriteBatch->end();
}
