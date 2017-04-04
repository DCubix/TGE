#ifndef TEXT_H
#define TEXT_H

#include "../ecs/tgComponent.h"
#include "../core/tgFont.h"

#define TEXT_LEN -1

enum tgTextAlign {
	tgALIGN_LEFT = 0,
	tgALIGN_CENTER,
	tgALIGN_RIGHT
};

typedef struct tgTextRange {
	tgTextRange() : start(0), length(1) {}

	int start, length;
	tgVector4 color = tgVector4(1.0f);
} tgTextRange;

typedef struct tgText : public tgComponent {
	friend class tgTextSystem;

	tgText() : m_text(""), m_font(nullptr), m_textAlign(tgALIGN_LEFT) {}
	tgText(std::string const& text, tgFont *font) : m_text(text), m_font(font), m_textAlign(tgALIGN_LEFT) {}

	std::string getText() const { return m_text; }
	void setText(std::string const& text) { m_text = text; }

	tgFont *getFont() const { return m_font; }
	void setFont(tgFont *font) { m_font = font; }

	tgTextAlign getTextAlign() const { return m_textAlign; }
	void setTextAlign(tgTextAlign const& ta) { m_textAlign = ta; }

	void colorize(tgVector4 const& color, int range_start=0, int range_len=TEXT_LEN);

	std::vector<tgTextRange> getRangeProperties() const { return m_rangeProperties; }

private:
	std::string m_text;
	tgFont *m_font;
	tgTextAlign m_textAlign;
	std::vector<tgTextRange> m_rangeProperties;
} tgText;

#endif
