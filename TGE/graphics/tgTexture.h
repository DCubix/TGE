#ifndef TEXTURE_H
#define TEXTURE_H

#include "tgGL.h"
#include "../core/tgImageData.h"

class tgTexture {
public:
	enum tgTextureType {
		tgTEXTURE_RGB = 0,		///< GL_RGB8
		tgTEXTURE_RGBA,			///< GL_RGBA8
		tgTEXTURE_FLOAT1,		///< GL_R16F
		tgTEXTURE_FLOAT2,		///< GL_RG16F
		tgTEXTURE_FLOAT3,		///< GL_RGB16F
		tgTEXTURE_FLOAT4,		///< GL_RGBA16F
		tgTEXTURE_FLOAT1_32,	///< GL_R32F
		tgTEXTURE_FLOAT2_32,	///< GL_RG32F
		tgTEXTURE_FLOAT3_32,	///< GL_RGB32F
		tgTEXTURE_FLOAT4_32,	///< GL_RGBA32F
		tgTEXTURE_DEPTH			///< GL_R16F
	};

	tgTexture(int width, int height, tgTextureType ttype);
	tgTexture(tgImageData &data, tgTextureType ttype);
	~tgTexture();

	void bind(int slot);
	void unbind();

	void resize(int nw, int nh);
	void setFilter(int min_filter, int mag_filter);
	void setWrap(int wrap);

	void generateMipMaps();

	GLuint getBindCode() const { return m_bindCode; }
	int getHeight() const { return m_height; }
	int getWidth() const { return m_width; }

private:
	GLuint m_bindCode;
	int m_width, m_height;
	tgTextureType m_type;

	void init(byte *data);
};

#endif // TEXTURE_H
