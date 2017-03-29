#ifndef FRAMEBUFFER_H
#define FRAMEBUGGER_H

#include "tgGL.h"
#include "tgTexture.h"

#include <vector>

class tgFrameBuffer {
public:
	enum tgFrameBufferBindMode {
		tgDRAW_FRAMEBUFFER = 0,
		tgREAD_FRAMEBUFFER,
		tgREAD_DRAW_FRAMEBUFFER
	};

	tgFrameBuffer();
	~tgFrameBuffer();

	void addColorAttachment(tgTexture *texture);
	void addDepthAttachment(tgTexture *texture);
	void addRenderBuffer(int width, int height, GLenum format, GLenum attachment);

	void setDrawBuffers(std::vector<GLenum> const& db);

	void bind(tgFrameBufferBindMode mode = tgREAD_DRAW_FRAMEBUFFER);
	void unbind(tgFrameBufferBindMode mode = tgREAD_DRAW_FRAMEBUFFER);

	GLuint getBindCode() const { return m_FBObindCode; }

	static void blit(int srcX0, int srcY0, int srcX1, int srcY1,
		int dstX0, int dstY0, int dstX1, int dstY1,
		int mask, int filter);
private:
	GLuint m_FBObindCode, m_RBObindCode, m_currentAttachment;
	int m_width, m_height;
};

#endif
