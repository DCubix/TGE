#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "../graphics/tgGL.h"
#include "../graphics/tgBuffer.h"
#include "../graphics/tgVertexArrayObject.h"
#include "../graphics/tgShaderProgram.h"
#include "../graphics/tgTexture.h"

class tgSpriteBatch {
public:
	tgSpriteBatch(int screen_width, int screen_height);
	~tgSpriteBatch();

	void draw(tgTexture *tex,
		tgVector4 const& uv, tgVector4 const& dst,
		tgVector2 const& origin, float rotation,
		tgVector4 const& color, float z);
	void drawTile(tgTexture *atlas, int tileIndex,
		int tileWidth, int tileHeight,
		int tileX, int tileY, float scale, float z);

	void resize(int w, int h);

	void begin();
	void end();

private:
	typedef struct tgVertex2D {
		tgVector3 position;
		tgVector2 texco;
		tgVector4 color;
	} tgVertex2D;

	typedef struct tgSprite {
		int texture;
		tgVertex2D TL, TR, BL, BR;
	} tgSprite;

	typedef struct tgBatch {
		int offset, numIndices, texture;

		tgBatch(int offset, int indices, int texture)
			: offset(offset), numIndices(indices), texture(texture)
		{}
	} tgBatch;

	tgBuffer *m_vbo, *m_ibo;
	tgVertexArrayObject *m_vao;
	std::vector<tgSprite*> m_sprites;
	std::vector<tgBatch> m_batches;
	int m_prevVBOSize, m_prevIBOSize;
	bool m_drawing;

	tgShaderProgram *m_shader;

	tgMatrix4 m_projection, m_view;

	void updateUniforms();
	void updateBuffers();

	void render();
};

#endif
