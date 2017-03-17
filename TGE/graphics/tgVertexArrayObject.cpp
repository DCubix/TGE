#include "tgVertexArrayObject.h"

#include <algorithm>
#include "../core/tgUtil.h"

tgVertexArrayObject::tgVertexArrayObject(tgVertexFormat const& fmt)
	: m_bindCode(0),
	m_creating(false),
	m_bound(false),
	m_vertexFormat(fmt) {
	GLCall(glGenVertexArrays(1, &m_bindCode));
}

tgVertexArrayObject::~tgVertexArrayObject() {
	if(m_bindCode) {
		GLCall(glDeleteVertexArrays(1, &m_bindCode));
		for(tgBuffer *buff : m_buffers) {
			if(buff) {
				delete buff;
			}
		}
		m_buffers.clear();
	}
}

void tgVertexArrayObject::append(tgBuffer* buff) {
	auto pos = std::find(m_buffers.begin(), m_buffers.end(), buff);
	if(pos == m_buffers.end() && !m_creating) {
		m_buffers.push_back(buff);
		m_creating = true;
	}
}

void tgVertexArrayObject::bind() {
	if(!m_bound) {
		GLCall(glBindVertexArray(m_bindCode));
		m_bound = true;
	}
}

void tgVertexArrayObject::unbind() {
	if(m_bound) {
		if(m_creating) {
			for(tgBuffer *buff : m_buffers) {
				if(buff) {
					buff->bind();
				}
			}
			if(!m_vertexFormat.isEmpty()) {
				m_vertexFormat.bind();
			}
		}
		m_bound = false;
		GLCall(glBindVertexArray(0));
	}
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
