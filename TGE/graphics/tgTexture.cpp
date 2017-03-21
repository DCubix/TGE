#include "tgTexture.h"

tgTexture::tgTexture(int width, int height, tgTextureType ttype)
	: m_bindCode(0),
	m_width(width),
	m_height(height),
	m_type(ttype) {
	init(nullptr);
	setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

tgTexture::tgTexture(tgImageData& data, tgTextureType ttype)
	: m_bindCode(0),
	m_width(data.getWidth()),
	m_height(data.getHeight()),
	m_type(ttype) {

	if(data.getData().size() > 0) {
		init(&data.getData()[0]);
	} else {
		init(nullptr);
	}
	setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
}

tgTexture::~tgTexture() {
	if(m_bindCode) {
		glDeleteTextures(1, &m_bindCode);
		m_bindCode = 0;
	}
}

void tgTexture::bind(int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
}

void tgTexture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void tgTexture::resize(int nw, int nh) {
	m_width = nw; m_height = nh;
	if(m_bindCode) {
		glDeleteTextures(1, &m_bindCode);
		m_bindCode = 0;
	}
	init(nullptr);
}

void tgTexture::setFilter(int min_filter, int mag_filter) {
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void tgTexture::setWrap(int wrap) {
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void tgTexture::generateMipMaps() {
	glBindTexture(GL_TEXTURE_2D, m_bindCode);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void tgTexture::init(byte* data) {
	if(m_bindCode <= 0) {
		glGenTextures(1, &m_bindCode);
	}

	glBindTexture(GL_TEXTURE_2D, m_bindCode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch(m_type) {
		case tgTEXTURE_RGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case tgTEXTURE_RGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case tgTEXTURE_DEPTH:
		case tgTEXTURE_FLOAT1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, m_width, m_height, 0, GL_R, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_width, m_height, 0, GL_RG, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT1_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0, GL_R, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT2_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, m_width, m_height, 0, GL_RG, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT3_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
			break;
		case tgTEXTURE_FLOAT4_32:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
			break;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
