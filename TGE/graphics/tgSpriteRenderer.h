#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "tgGL.h"
#include "tgBuffer.h"
#include "tgVertexArrayObject.h"
#include "tgShaderProgram.h"
#include "tgTexture.h"

#include "../core/tgUtil.h"

#include <vector>

class tgSpriteRenderer {
public:
	tgSpriteRenderer(int screen_width, int screen_height);
	~tgSpriteRenderer();

	void draw(tgTexture *tex,
		tgVector4 const& uv, tgVector4 const& dst,
		tgVector2 const& origin, float rotation);

	void begin();
	void end();
	void render();

	void resize(int w, int h);

private:
	typedef struct tgVertex2D {
		tgVector3 position;
		tgVector2 texco;
	} tgVertex2D;

	typedef struct tgSprite {
		int texture;
		tgVertex2D TL, TR, BL, BR;
	} tgSprite;

	typedef struct tgBatch {
		int offset, numVertices, texture;
	} tgBatch;

	tgBuffer *m_vbo;
	tgVertexArrayObject *m_vao;
	std::vector<tgSprite> m_sprites;
	std::vector<tgBatch> m_batches;
	int m_prevVBOSize, m_prevIBOSize;
	bool m_drawing;

	tgShaderProgram *m_shader;

	tgMatrix4 m_projection, m_view;
	
	void updateUniforms();
	void updateBuffers();

};

#endif // SPRITE_RENDERER_H
