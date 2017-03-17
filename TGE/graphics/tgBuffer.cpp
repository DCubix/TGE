#include "tgBuffer.h"

#include "../core/tgUtil.h"

tgBuffer::tgBuffer(tgBufferTarget target, tgBufferUsage usage)
	: m_bindCode(0),
	m_target(target),
	m_usage(usage) {
	GLCall(glGenBuffers(1, &m_bindCode));
}

tgBuffer::~tgBuffer() {
	if(m_bindCode) {
		GLCall(glDeleteBuffers(1, &m_bindCode));
	}
}

void tgBuffer::bind() {
	GLCall(glBindBuffer(m_target, m_bindCode));
}

void tgBuffer::unbind() {
	GLCall(glBindBuffer(m_target, 0));
}

void tgBuffer::reserve(size_t size) {
	GLCall(glBufferData(m_target, size, nullptr, m_usage));
}

void tgBuffer::setData(void* data, size_t data_size) {
	GLCall(glBufferData(m_target, data_size, data, m_usage));
}

void tgBuffer::update(unsigned offset, void* data, size_t data_size) {
	GLCall(glBufferSubData(m_target, offset, data_size, data));
}
