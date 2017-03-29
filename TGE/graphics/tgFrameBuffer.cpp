#include "tgFrameBuffer.h"

#include "../core/tgUtil.h"
#include "../core/tgLog.h"
#include <algorithm>

static const char* getFramebufferError(GLenum status) {
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE: return "";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "Incomplete attachment";
		case GL_FRAMEBUFFER_UNSUPPORTED: return "Unsupported framebuffer format";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "Missing attachment";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "Missing draw buffer";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "Missing read buffer";
	}
	return "Unknown error";
}

tgFrameBuffer::tgFrameBuffer()
	: m_currentAttachment(0),
	m_width(0),
	m_height(0),
	m_FBObindCode(0),
	m_RBObindCode(0)
{
	GLCall(glGenFramebuffers(1, &m_FBObindCode));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBObindCode));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

tgFrameBuffer::~tgFrameBuffer() {
	if (m_FBObindCode) {
		GLCall(glDeleteFramebuffers(1, &m_FBObindCode));
	}
	if (m_RBObindCode) {
		GLCall(glDeleteRenderbuffers(1, &m_RBObindCode));
	}
}

void tgFrameBuffer::addColorAttachment(tgTexture *texture) {
	if (!texture) {
		return;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBObindCode));

	m_width = std::max(m_width, texture->getWidth());
	m_height = std::max(m_height, texture->getHeight());

	GLCall(glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + m_currentAttachment,
		GL_TEXTURE_2D,
		texture->getBindCode(),
		0
	));

	GLenum *drawBuffers = new GLenum[m_currentAttachment + 1];
	for (int i = 0; i < m_currentAttachment + 1; i++) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	GLCall(glDrawBuffers(m_currentAttachment + 1, drawBuffers));

	m_currentAttachment++;

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		tgLog::log("Framebuffer error(", status ,"): ", getFramebufferError(status));
		GLCall(glDeleteFramebuffers(1, &m_FBObindCode));
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void tgFrameBuffer::addDepthAttachment(tgTexture *texture) {
	if (!texture) {
		return;
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBObindCode));

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		texture->getBindCode(),
		0
	);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		tgLog::log("Framebuffer error(", status, "): ", getFramebufferError(status));
		GLCall(glDeleteFramebuffers(1, &m_FBObindCode));
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void tgFrameBuffer::addRenderBuffer(int width, int height, GLenum format, GLenum attachment) {
	if (!m_RBObindCode) {
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBObindCode));
		GLCall(glGenRenderbuffers(1, &m_RBObindCode));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBObindCode));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_RBObindCode));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}

void tgFrameBuffer::setDrawBuffers(std::vector<GLenum> const& db) {
	glDrawBuffers(db.size(), &db[0]);
}

void tgFrameBuffer::bind(tgFrameBufferBindMode mode) {
	switch (mode) {
		case tgREAD_FRAMEBUFFER: glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBObindCode); break;
		case tgDRAW_FRAMEBUFFER: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBObindCode); break;
		case tgREAD_DRAW_FRAMEBUFFER: glBindFramebuffer(GL_FRAMEBUFFER, m_FBObindCode); glViewport(0, 0, m_width, m_height);  break;
	}
}

void tgFrameBuffer::unbind(tgFrameBufferBindMode mode) {
	switch (mode) {
		case tgREAD_FRAMEBUFFER: glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); break;
		case tgDRAW_FRAMEBUFFER: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); break;
		case tgREAD_DRAW_FRAMEBUFFER: glBindFramebuffer(GL_FRAMEBUFFER, 0); break;
	}
}

void tgFrameBuffer::blit(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, int mask, int filter) {
	glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}
