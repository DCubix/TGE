#ifndef BUFFER_H
#define BUFFER_H

#include "tgGL.h"

#include <cstddef>

class tgBuffer {
public:
	enum tgBufferTarget {
		tgTARG_ARRAY_BUFFER = GL_ARRAY_BUFFER,
		tgTARG_INDEX_BUFFER = GL_ELEMENT_ARRAY_BUFFER
	};

	enum tgBufferUsage {
		tgUSAGE_STATIC = GL_STATIC_DRAW,
		tgUSAGE_DYNAMIC = GL_DYNAMIC_DRAW
	};

	tgBuffer(tgBufferTarget target, tgBufferUsage usage);
	~tgBuffer();

	void bind();
	void unbind();

	void reserve(std::size_t size);
	void setData(void *data, std::size_t data_size);
	void update(unsigned offset, void *data, std::size_t data_size);

	GLuint getBindCode() const { return m_bindCode; }

private:
	GLuint m_bindCode;
	tgBufferTarget m_target;
	tgBufferUsage m_usage;
};

#endif // BUFFER_H
