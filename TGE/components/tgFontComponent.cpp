#include "tgFontComponent.h"

#include "../ecs/tgComponentManager.h"
#include "../components/tgTransformComponent.h"

#include <cctype>

static float computeTextWidth(tgFontComponent *fcomp, tgVector2 const& scale) {
	tgFont *fnt = fcomp->getFont();
	float w = 0;
	float padX = fnt->getPadding().x();
	float xscale = scale.x();
	float yscale = scale.y();
	for (unsigned i = 0; i < fcomp->getText().size(); i++) {
		char c = fcomp->getText()[i];
		tgChar chr;
		if (fnt->containsChar(c)) {
			chr = fnt->getCharMap()[c];
		} else {
			chr = fnt->getCharMap()[char(fnt->getNumChars() - 1)];
		}
		w += ((chr.xadvance - padX) * xscale) + (chr.xoffset * xscale);
	}
	return w;
}

void tgFontComponent::render(tgRenderer *renderer) {
	tgSpriteBatch *sb = dynamic_cast<tgSpriteBatch*>(renderer);
	if (!sb) { return; }

	tgTransformComponent *transform = getManager()->getComponent<tgTransformComponent>(getOwner());

	tgVector3 pos(0.0f);
	tgVector2 scl(1.0f);
	if (transform) {
		pos = transform->getTransform()->getWorldPosition();
		scl = transform->getTransform()->getWorldScaling().xy();
	}

	tgFont *fnt = m_font;
	tgTexture *tex = fnt ? fnt->getTexture() : nullptr;
	if (fnt && tex) {
		sb->save();

		float xscale = scl.x();
		float yscale = scl.y();
		float x = 0;

		for (unsigned i = 0; i < m_text.size(); i++) {
			char c = m_text[i];
			tgChar chr;
			if (fnt->containsChar(c)) {
				chr = fnt->getCharMap()[c];
			} else {
				chr = fnt->getCharMap()[char(fnt->getNumChars() - 1)];
			}
			if (!std::isspace(c)) {
				sb->setUV(chr.clipRect);
				sb->setPosition(
					tgVector3(pos.x() + (x + chr.xoffset * xscale) - fnt->getPadding()[0],
						pos.y() + (chr.yoffset * yscale), pos.z())
				);
				sb->setScale(scl);
				sb->draw(fnt->getTexture());
			}
			x += (chr.xadvance - fnt->getPadding()[2]) * xscale;
		}
		
		sb->restore();
	}
}
