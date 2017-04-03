#ifndef TEXT_H
#define TEXT_H

#include "../ecs/tgComponent.h"
#include "../core/tgFont.h"

typedef struct tgText : public tgComponent {
	friend class tgTextSystem;

	tgText() : m_text(""), m_font(nullptr) {}
	tgText(std::string const& text, tgFont *font) : m_text(text), m_font(font) {}

	std::string getText() const { return m_text; }
	void setText(std::string const& text) { m_text = text; }

	tgFont *getFont() const { return m_font; }
	void setFont(tgFont *font) { m_font = font; }

private:
	std::string m_text;
	tgFont *m_font;
} tgText;

#endif
