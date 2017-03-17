#include "tgBuffer.h"

#include "../core/tgUtil.h"

tgBuffer::tgBuffer(tgBufferTarget target, tgBufferUsage usage)
	: m_bindCode(0),
	m_target(target),
	m_usage(usage),
	m_bound(false) {
	GLCall(glGenBuffers(1, &m_bindCode));
}

tgBuffer::~tgBuffer() {
	if(m_bindCode) {
		GLCall(glDeleteBuffers(1, &m_bindCode));
	}
}

void tgBuffer::bind() {
	if(!m_bound) {
		GLCall(glBindBuffer(m_target, m_bindCode));
		m_bound = true;
	}
}

void tgBuffer::unbind() {
	if(m_bound) {
		GLCall(glBindBuffer(m_target, 0));
		m_bound = false;
	}
}

void tgBuffer::reserve(size_t size) {
	bind();
	GLCall(glBufferData(m_target, size, nullptr, m_usage));
}

void tgBuffer::setData(void* data, size_t data_size) {
	bind();
	GLCall(glBufferData(m_target, data_size, data, m_usage));
}

void tgBuffer::update(unsigned offset, void* data, size_t data_size) {
	bind();
	GLCall(glBufferSubData(m_target, offset, data_size, data));
}
