#ifndef RENDERER_H
#define RENDERER_H

#include "../math/tgMath.h"

#include "tgTexture.h"
#include "tgShaderProgram.h"
#include "tgVertexArrayObject.h"
#include "tgVertexFormat.h"
#include "tgBuffer.h"

#include <vector>
#include <functional>

#define TG_MAX_TEXTURES 8

typedef struct tgVertex {
	tgVector3 position, normal, tangent;
	tgVector4 color;
	tgVector2 texCoord;

	tgVertex() {}
	tgVertex(tgVector3 position, tgVector3 normal, tgVector3 tangent, tgVector4 color, tgVector2 uv)
		: position(position), normal(normal), tangent(tangent), color(color)
	{
		texCoord = uv;
	}
	tgVertex(tgVector3 position, tgVector2 uv, tgVector4 color)
		: position(position), normal(tgVector3(0, 0, 1)), tangent(tgVector3(0, 1, 0)), color(color)
	{
		texCoord = uv;
	}
} tgVertex;

enum tgTextureSlotType {
	tgALBEDO_TEXTURE = 0,
	tgNORMAL_TEXTURE,
	tgMATERIAL_PARAM0,
	tgMATERIAL_PARAM1,
	tgMATERIAL_PARAM2,
	tgMATERIAL_PARAM3
};

typedef struct tgTextureSlot {
	int id;
	tgTextureSlotType type;
} tgTextureSlot;

enum tgBlendMode {
	tgBLEND_NORMAL = 0,
	tgBLEND_ADD
};

typedef struct tgMesh {
	std::vector<tgVertex> vertices;
	std::vector<int> indices;
	tgTextureSlot texture;
	tgMatrix4 modelMatrix;
	tgBlendMode blendMode;
} tgMesh;

typedef struct tgMeshBatch {
	int offset, numIndices;
	tgTextureSlot texture;
	tgMatrix4 modelMatrix;
	tgBlendMode blendMode;
	tgMeshBatch(int off, int n, tgTextureSlot texture, tgMatrix4 const& modelMatrix, tgBlendMode blendMode)
		: offset(off), numIndices(n), texture(texture), modelMatrix(modelMatrix), blendMode(blendMode)
	{}
} tgMeshBatch;

class tgRenderer {
public:
	tgRenderer() {}
	tgRenderer(int screen_width, int screen_height);
	virtual ~tgRenderer() = default;
	
	void submit(tgMesh *mesh);

	virtual void begin();
	virtual void end() {}

	virtual void render() {}
	virtual void renderGeometry(tgShaderProgram *shader) {}

	void clear(int flags);
protected:
	std::vector<tgMesh*> m_meshes;
	tgMatrix4 m_projection, m_view;
};

#endif
