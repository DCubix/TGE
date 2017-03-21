#ifndef SPRITE_RENDERING_SYSTEM_H
#define SPRITE_RENDERING_SYSTEM_H

#include "../graphics/tgGL.h"
#include "../graphics/tgBuffer.h"
#include "../graphics/tgVertexArrayObject.h"
#include "../graphics/tgShaderProgram.h"
#include "../graphics/tgTexture.h"

#include "../ecs/tgSystem.h"

#include <vector>

class tgSpriteRenderingSystem : public tgSystem {
public:
	tgSpriteRenderingSystem(int screen_width, int screen_height);
	~tgSpriteRenderingSystem() override final;

	void render(tgEntitySystemManager *mgr) override final;
	void resize(int w, int h);

	void draw(tgTexture *tex,
		tgVector4 const& uv, tgVector4 const& dst,
		tgVector2 const& origin, float rotation);
	void drawTile(tgTexture *atlas, int tileIndex,
		int tileWidth, int tileHeight,
		int tileX, int tileY, float scale);

protected:
	typedef struct tgVertex2D {
		tgVector3 position;
		tgVector2 texco;
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

	void begin();
	void end();
	void render();

};

#endif
