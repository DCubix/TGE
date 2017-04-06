#include "tgTextSystem.h"

#include "../ecs/tgECS.h"
#include "../components/tgText.h"
#include "../components/tgTransform.h"
#include "../graphics/tgRenderer.h"

#include <cctype>

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

static std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

static float computeTextWidth(std::string const& text, tgFont *fnt, tgVector2 const& scale) {
	float w = 0;
	float padX = fnt->getPadding()[2];
	float xscale = scale.x();
	for (unsigned i = 0; i < text.size(); i++) {
		char c = text[i];
		tgChar chr;
		if (fnt->containsChar(c)) {
			chr = fnt->getCharMap()[c];
		} else {
			chr = fnt->getCharMap()[char(fnt->getNumChars() - 1)];
		}
		w += float(chr.xadvance - fnt->getPadding()[2]) * xscale;
	}
	return w;
}

void tgTextSystem::render(tgRenderer *renderer) {
	for (tgEntity *ent : getECS()->with<tgText, tgTransform>()) {
		auto t = ent->get<tgText>();
		auto transform = ent->get<tgTransform>();

		tgVector3 pos(0.0f);
		tgVector2 scl(1.0f);
		if (transform) {
			pos = transform->getWorldPosition();
			scl = transform->getWorldScaling().xy();
		}

		tgFont *fnt = t->m_font;
		tgTexture *tex = fnt ? fnt->getTexture() : nullptr;
		if (fnt && tex) {
			renderer->save();

			float xscale = scl.x();
			float yscale = scl.y();
			float x = 0;
			float y = 0;

			std::vector<std::string> lines = split(t->m_text, '\n');

			int char_index = 0;
			for (std::string text : lines) {
				switch (t->getTextAlign()) {
					case tgTextAlign::tgALIGN_LEFT: x = 0; break;
					case tgTextAlign::tgALIGN_CENTER: x = -computeTextWidth(text, fnt, scl) / 2; break;
					case tgTextAlign::tgALIGN_RIGHT: x = -computeTextWidth(text, fnt, scl); break;
				}

				for (unsigned i = 0; i < text.size(); i++) {
					char c = text[i];
					tgChar chr;

					if (fnt->containsChar(c)) {
						chr = fnt->getCharMap()[c];
					} else {
						chr = fnt->getCharMap()[char(fnt->getNumChars() - 1)];
					}
					if (!std::isspace(c)) {
						renderer->setUV(chr.clipRect);
						renderer->setPosition(
							tgVector3(pos.x() + (x + chr.xoffset * xscale) - fnt->getPadding()[0],
									  pos.y() + (y + chr.yoffset * yscale) - fnt->getPadding()[1],
									  pos.z())
						);
						renderer->setScale(scl);

						renderer->setColor(tgVector4(1.0f));
						for (tgTextRange &tr : t->getRangeProperties()) {
							if (char_index >= tr.start && char_index <= tr.start + tr.length - 1) {
								renderer->setColor(tr.color);
							}
						}

						renderer->draw(fnt->getTexture());
					}
					if (c != '\n') {
						char_index++;
					}
					x += (chr.xadvance - fnt->getPadding()[2]) * xscale;
				}
				y += (fnt->getLineHeight() - fnt->getPadding()[3]) * yscale;
			}
			renderer->restore();
		}
	}
}
