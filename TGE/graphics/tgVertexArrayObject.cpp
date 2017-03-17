#include "tgVertexArrayObject.h"

#include <algorithm>
#include "../core/tgUtil.h"

tgVertexArrayObject::tgVertexArrayObject()
	: m_bindCode(0) {
	GLCall(glGenVertexArrays(1, &m_bindCode));
}

tgVertexArrayObject::~tgVertexArrayObject() {
	if(m_bindCode) {
		GLCall(glDeleteVertexArrays(1, &m_bindCode));
	}
}

void tgVertexArrayObject::bind() {
	GLCall(glBindVertexArray(m_bindCode));
}

void tgVertexArrayObject::unbind() {
	GLCall(glBindVertexArray(0));
}

void tgVertexArrayObject::drawArrays(tgPrimitiveType primitive, int first, int count) {
	GLCall(glDrawArrays(primitive, first, count));
}

void tgVertexArrayObject::drawElements(tgPrimitiveType primitive, int count, void* indices) {
	GLCall(glDrawElements(primitive, count, GL_UNSIGNED_INT, indices));
}

void tgVertexArrayObject::drawElements(tgPrimitiveType primitive, int count, int offset) {
	GLCall(glDrawElements(primitive, count, GL_UNSIGNED_INT, (void*)offset));
}
