#ifndef FONT_COMPONENT_H
#define FONT_COMPONENT_H

#include "../ecs/tgComponent.h"
#include "../core/tgFont.h"
#include "../graphics/tgSpriteBatch.h"

#include <string>

class tgFontComponent : public tgComponent {
public:
	tgFontComponent(tgSpriteBatch *sb) : m_spriteBatch(sb), m_text(""), m_font(nullptr) {}
	tgFontComponent(tgSpriteBatch *sb, std::string const& text, tgFont *font) : m_spriteBatch(sb), m_text(text), m_font(font) {}

	std::string getText() const { return m_text; }
	void setText(std::string const& text) { m_text = text; }

	tgFont *getFont() const { return m_font; }
	void setFont(tgFont *font) { m_font = font; }

	void render() override;

private:
	tgSpriteBatch *m_spriteBatch;
	std::string m_text;
	tgFont *m_font;
};

#endif