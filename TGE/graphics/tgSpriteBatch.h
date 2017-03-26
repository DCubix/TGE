#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "../graphics/tgGL.h"
#include "../graphics/tgBuffer.h"
#include "../graphics/tgVertexArrayObject.h"
#include "../graphics/tgShaderProgram.h"
#include "../graphics/tgTexture.h"

class tgSpriteBatch {
public:
	enum tgBlendMode {
		tgBLEND_NORMAL = 0,
		tgBLEND_ADD
	};

	tgSpriteBatch(int screen_width, int screen_height);
	~tgSpriteBatch();

	void draw(tgTexture *tex);

	void save();
	void setPosition(tgVector3 const& pos) { m_currentState.position = pos; }
	void setScale(tgVector2 const& scl) { m_currentState.scale = scl; }
	void setUV(tgVector4 const& uv) { m_currentState.uv = uv; }
	void setOrigin(tgVector2 const& ori) { m_currentState.origin = ori; }
	void setColor(tgVector4 const& col) { m_currentState.color = col; }
	void setRotation(float rot) { m_currentState.rotation = rot; }
	void setBlendMode(tgBlendMode blendMode) { m_currentState.blendMode = blendMode; }
	void restore();

	void resize(int w, int h);

	void begin();
	void end();

private:
	typedef struct tgSpriteBatchState {
		tgVector3 position		= tgVector3();
		tgVector2 scale			= tgVector2(1.0f);
		tgVector4 uv			= tgVector4(0, 0, 1, 1);
		tgVector2 origin		= tgVector2();
		tgVector4 color			= tgVector4(1.0f);
		tgBlendMode blendMode	= tgBlendMode::tgBLEND_NORMAL;
		float rotation			= 0;

		tgSpriteBatchState *clone() {
			tgSpriteBatchState *st = new tgSpriteBatchState();
			st->position = tgVector3(position.x(), position.y(), position.z());
			st->scale = tgVector2(scale.x(), scale.y());
			st->uv = tgVector4(uv.x(), uv.y(), uv.z(), uv.w());
			st->origin = tgVector2(origin.x(), origin.y());
			st->color = tgVector4(color.x(), color.y(), color.z(), color.w());
			st->rotation = rotation;
			st->blendMode = blendMode;
			return st;
		}
	} tgSpriteBatchState;

	typedef struct tgVertex2D {
		tgVector2 position;
		tgVector2 texco;
		tgVector4 color;
	} tgVertex2D;

	typedef struct tgSprite {
		int texture;
		tgVertex2D TL, TR, BL, BR;
		tgBlendMode blend;
		float z;
	} tgSprite;

	typedef struct tgBatch {
		int offset, numIndices, texture;
		float z;
		tgBlendMode blend;

		tgBatch(int offset, int indices, int texture, tgBlendMode blend, float z)
			: offset(offset), numIndices(indices), texture(texture), blend(blend), z(z)
		{}
	} tgBatch;

	tgBuffer *m_vbo, *m_ibo;
	tgVertexArrayObject *m_vao;

	std::vector<tgSprite*> m_sprites;
	std::vector<tgBatch> m_batches;

	tgSpriteBatchState m_prevState;
	tgSpriteBatchState m_currentState;

	int m_prevVBOSize, m_prevIBOSize;
	bool m_drawing;

	tgShaderProgram *m_shader;

	tgMatrix4 m_projection, m_view;

	void updateUniforms();
	void updateBuffers();

	void render();
};

#endif
