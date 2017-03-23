#include "tgFont.h"

bool tgFont::containsChar(char c) const {
	auto pos = m_charMap.find(c);
	return pos != m_charMap.end();
}
