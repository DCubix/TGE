#ifndef FONT_H
#define FONT_H

#include "../math/tgMath.h"
#include "../graphics/tgTexture.h"

#include <unordered_map>

typedef struct tgChar {
	int id;
	float xoffset, yoffset, xadvance;
	tgVector4 clipRect;

	tgChar() {
		clipRect = tgVector4(0, 0, 1, 1);
		xoffset = 0;
		yoffset = 0;
		xadvance = 0;
	}
} tgChar;

using tgCharMap = std::unordered_map<char, tgChar>;

class tgFontAsset;
class tgFont {
	friend class tgFontAsset;
public:
	tgFont() {}

	float getMapWidth() const { return m_mapWidth; }
	float getMapHeight() const { return m_mapHeight; }
	float getLineHeight() const { return m_lineHeight; }
	int getNumChars() const { return m_numChars; }
	tgVector4 getPadding() const { return m_padding; }
	tgTexture *getTexture() const { return m_texture; }
	tgCharMap getCharMap() const { return m_charMap; }

	bool containsChar(char c) const;

protected:
	float m_mapWidth, m_mapHeight, m_lineHeight;
	int m_numChars;
	tgVector4 m_padding;

	tgTexture *m_texture;

	tgCharMap m_charMap;
};

#endif
