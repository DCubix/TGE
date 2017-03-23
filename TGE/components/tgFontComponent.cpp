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
	for (int i = 0; i < fcomp->getText().size(); i++) {
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

void tgFontComponent::render() {
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
		float xscale = scl.x();
		float yscale = scl.y();
		float x = 0;

		for (int i = 0; i < m_text.size(); i++) {
			char c = m_text[i];
			tgChar chr;
			if (fnt->containsChar(c)) {
				chr = fnt->getCharMap()[c];
			} else {
				chr = fnt->getCharMap()[char(fnt->getNumChars() - 1)];
			}
			if (!std::isspace(c)) {
				m_spriteBatch->draw(
					fnt->getTexture(),
					chr.clipRect,
					tgVector4(pos.x() + (x + chr.xoffset * xscale) - fnt->getPadding()[0], pos.y() + (chr.yoffset * yscale),
						float(tex->getWidth()) * xscale, float(tex->getHeight()) * yscale),
					tgVector2(0.0f),
					0,
					tgVector4(1.0f), pos.z()
				);
			}
			x += (chr.xadvance - fnt->getPadding()[2]) * xscale;
		}
		x = 0;
	}
}