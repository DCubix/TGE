#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include "tgBuffer.h"
#include "tgVertexFormat.h"

#include <vector>

class tgVertexArrayObject {
public:
	enum tgPrimitiveType {
		tgPRIM_TRIANGLES = GL_TRIANGLES,
		tgPRIM_TRIANGLE_FAN = GL_TRIANGLE_FAN,
		tgPRIM_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		tgPRIM_POINTS = GL_POINTS,
		tgPRIM_LINES = GL_LINES
	};

	tgVertexArrayObject();
	~tgVertexArrayObject();

	void bind();
	void unbind();

	void drawArrays(tgPrimitiveType primitive, int first, int count);
	void drawElements(tgPrimitiveType primitive, int count, void *indices);
	void drawElements(tgPrimitiveType primitive, int count, int offset);

	GLuint getBindCode() const { return m_bindCode; }

private:
	GLuint m_bindCode;
};

#endif // VERTEX_ARRAY_OBJECT_H
