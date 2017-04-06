#ifndef RENDERER_H
#define RENDERER_H

#include "../math/tgMath.h"

#include "tgTexture.h"
#include "tgFrameBuffer.h"
#include "tgShaderProgram.h"
#include "tgPostEffect.h"

#include <vector>
#include <functional>

typedef struct tgVertex {
	tgVector3 position, normal;
	tgVector4 color;
	tgVector2 texCoord;

	tgVertex() {}
	tgVertex(tgVector3 position, tgVector3 normal, tgVector3 tangent, tgVector4 color, tgVector2 uv)
		: position(position), normal(normal), color(color)
	{
		texCoord = uv;
	}
	tgVertex(tgVector3 position, tgVector2 uv, tgVector4 color)
		: position(position), normal(tgVector3(0, 0, 1)), color(color)
	{
		texCoord = uv;
	}
} tgVertex;

enum tgTextureSlotType {
	tgCOLOR_TEXTURE = 0,
	tgNORMAL_TEXTURE,
	tgMATERIAL_PARAM0,
	tgMATERIAL_PARAM1
};

typedef struct tgTextureSlot {
	int id;
	tgTextureSlotType type;
} tgTextureSlot;

enum tgBlendMode {
	tgBLEND_NORMAL = 0,
	tgBLEND_ADD
};

class tgRenderer {
public:
	tgRenderer() {}
	tgRenderer(int screen_width, int screen_height);
	virtual ~tgRenderer();
	
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

	void render();
	void renderGeometry(tgShaderProgram *shader);

	void beginPostFX();
	void postProcess();
	void endPostFX();

	void basicPostProcess();

	void addPostEffect(tgPostEffect *fx);
	void removePostEffect(int passIndex);

	void clear(int flags);
	void setClearColor(tgVector4 const& color);

protected:
	typedef struct tgSprite {
		tgVertex tl, tr, bl, br;
		tgTextureSlot textures[2];
		tgBlendMode blendMode;
	} tgSprite;

	typedef struct tgBatch {
		int offset, numIndices;
		tgTextureSlot textures[2];
		tgBlendMode blendMode;
		float z;
		tgBatch(int off, int n, tgTextureSlot texture, tgBlendMode blendMode, float z)
			: offset(off), numIndices(n), blendMode(blendMode), z(z)
		{
			textures[0] = texture;
			textures[1] = { -1, tgCOLOR_TEXTURE };
		}
		tgBatch(int off, int n, tgTextureSlot *textures, tgBlendMode blendMode, float z)
			: offset(off), numIndices(n), blendMode(blendMode), z(z)
		{
			this->textures[0] = textures[0];
			this->textures[1] = textures[1];
		}
	} tgBatch;

	typedef struct tgSpriteBatchState {
		tgVector3 position = tgVector3();
		tgVector2 scale = tgVector2(1.0f);
		tgVector4 uv = tgVector4(0, 0, 1, 1);
		tgVector2 origin = tgVector2();
		tgVector4 color = tgVector4(1.0f);
		tgBlendMode blendMode = tgBlendMode::tgBLEND_NORMAL;
		float rotation = 0;

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

	std::vector<tgSprite*> m_sprites;
	std::vector<tgBatch> m_batches;
	
	std::vector<tgPostEffect*> m_postfx;
	tgTexture *m_pingpong[2];
	tgTexture *m_finalTexture;
	tgFrameBuffer *m_pingPongBuffer, *m_finalBuffer;
	tgShaderProgram *m_defaultShader;

	GLuint m_qvao, m_vao;
	GLuint m_qvbo, m_vbo, m_ibo;

	tgSpriteBatchState m_prevState;
	tgSpriteBatchState m_currentState;

	int m_prevVBOSize, m_prevIBOSize;
	bool m_drawing;

	tgShaderProgram *m_shader;

	tgMatrix4 m_projection, m_view;

	int w, h;

	void submit(tgSprite *sprite);
	void updateUniforms();
	void updateBuffers();
	void drawQuad();
};

#endif
