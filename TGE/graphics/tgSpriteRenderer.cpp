#include "tgSpriteRenderer.h"

#include <string>
#include <cmath>

tgSpriteRenderer::tgSpriteRenderer(int screen_width, int screen_height)
	: m_vbo(nullptr),
	m_vao(nullptr),
	m_prevVBOSize(0),
	m_prevIBOSize(0),
	m_drawing(false) {

	tgVertexFormat fmt(sizeof(tgVertex2D));
	fmt.append(tgVertexFormat::tgATTR_POSITION, false);
	fmt.append(tgVertexFormat::tgATTR_TEXCOORD, false);

	m_vao = new tgVertexArrayObject();
	m_vbo = new tgBuffer(tgBuffer::tgTARG_ARRAY_BUFFER, tgBuffer::tgUSAGE_DYNAMIC);
	m_ibo = new tgBuffer(tgBuffer::tgTARG_INDEX_BUFFER, tgBuffer::tgUSAGE_DYNAMIC);

	m_vao->bind();
	m_vbo->bind();
	fmt.bind();

	m_ibo->bind();

	m_vao->unbind();

	std::string sb_vert =
		"#version 440\n"
		"layout (location = 0) in vec3 v_pos;"
		"layout (location = 1) in vec2 v_uv;"
		"out DATA {"
		"	vec3 position;"
		"	vec2 uv;"
		"} vs_out;"
		"uniform mat4 viewProjection = mat4(1.0);"
		"void main() {"
		"	gl_Position = viewProjection * vec4(v_pos, 1.0);"
		"	vs_out.position = v_pos;"
		"	vs_out.uv = v_uv;"
		"}";
	std::string sb_frag =
		"#version 440\n"
		"out vec4 fragColor;"
		"in DATA {"
		"	vec3 position;"
		"	vec2 uv;"
		"} fs_in;"
		"uniform sampler2D tex0;"
		"void main() {"
		"	fragColor = texture(tex0, fs_in.uv);"
		"}";
	m_shader = new tgShaderProgram();
	m_shader->addShader(sb_vert, tgShaderProgram::tgVERTEX_SHADER);
	m_shader->addShader(sb_frag, tgShaderProgram::tgFRAGMENT_SHADER);
	m_shader->link();

	m_projection = tgMatrix4::ortho(0, float(screen_width), float(screen_height), 0, -1, 1);
	m_view = tgMatrix4::identity();
	
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	updateUniforms();
}

tgSpriteRenderer::~tgSpriteRenderer() {
	tgDelete(m_ibo);
	tgDelete(m_vbo);
	tgDelete(m_vao);
	tgDelete(m_shader);
}

void tgSpriteRenderer::begin() {
	if(!m_drawing) {
		m_drawing = true;
		m_shader->bind();
		m_sprites.clear();
		m_batches.clear();
	}
}

void tgSpriteRenderer::end() {
	if(m_drawing) {
		m_drawing = false;
		updateBuffers();
		render();
		m_shader->unbind();
	}
}

void tgSpriteRenderer::render() {
	bool shouldUnbind = false;
	m_vao->bind();
	for(tgBatch batch : m_batches) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, batch.texture);

		//m_vao->drawArrays(tgVertexArrayObject::tgPRIM_TRIANGLES, batch.offset, batch.numVertices);
		m_vao->drawElements(tgVertexArrayObject::tgPRIM_TRIANGLES, batch.numIndices, batch.offset);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	m_vao->unbind();
}

void tgSpriteRenderer::resize(int w, int h) {
	m_projection = tgMatrix4::ortho(0, float(w), float(h), 0, -1, 1);
	updateUniforms();
}

void tgSpriteRenderer::updateUniforms() {
	m_shader->bind();
	m_shader->setMatrix4("viewProjection", m_projection * m_view);
	m_shader->setInt("tex0", 0);
}

void tgSpriteRenderer::updateBuffers() {
	if(m_sprites.size() == 0) {
		return;
	}

	std::vector<tgVertex2D> vertices;
	vertices.reserve(m_sprites.size() * 4);

	std::vector<int> indices;
	indices.reserve(m_sprites.size() * 6);

	tgSprite spr0 = m_sprites[0];
	int indexOffset = 0, offset = 0;
	int indexCount = 0;
	int prevtex = spr0.texture;

	vertices.push_back(spr0.BL);
	vertices.push_back(spr0.TL);
	vertices.push_back(spr0.TR);
	vertices.push_back(spr0.BR);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	indexCount += 6;
	indexOffset += 4;

	for(std::size_t i = 1; i < m_sprites.size(); i++) {
		tgSprite spr = m_sprites[i];

		vertices.push_back(spr.BL);
		vertices.push_back(spr.TL);
		vertices.push_back(spr.TR);
		vertices.push_back(spr.BR);

		indices.push_back(0 + indexOffset);
		indices.push_back(1 + indexOffset);
		indices.push_back(2 + indexOffset);
		indices.push_back(2 + indexOffset);
		indices.push_back(3 + indexOffset);
		indices.push_back(0 + indexOffset);

		indexCount += 6;
		indexOffset += 4;

		if(prevtex != spr.texture || i >= m_sprites.size()-1) {
			tgBatch b;
			b.numIndices = indexCount;
			b.offset = offset;
			b.texture = spr.texture;
			m_batches.push_back(b);

			offset += indexCount;
			indexCount = 0;
		}

		prevtex = spr.texture;
	}

	m_vbo->bind();
	int vboSize = vertices.size() * sizeof(tgVertex2D);
	if(vboSize > m_prevVBOSize) {
		m_vbo->reserve(vboSize);
		m_prevVBOSize = vboSize;
	}
	m_vbo->update(0, &vertices[0], vboSize);

	m_ibo->bind();
	int iboSize = indices.size() * sizeof(int);
	if(iboSize > m_prevIBOSize) {
		m_ibo->reserve(iboSize);
		m_prevIBOSize = iboSize;
	}
	m_ibo->update(0, &indices[0], iboSize);
}

static tgVector2 rotatePoint(tgVector2 const& p, float rad) {
	float c = std::cos(rad);
	float s = std::sin(rad);
	return tgVector2(c * p.x() - s * p.y(), s * p.x() + c * p.y());
}

void tgSpriteRenderer::draw(tgTexture* tex,
	tgVector4 const& uv, tgVector4 const& dst,
	tgVector2 const& origin, float rotation)
{
	float width = dst.z() * uv.z();
	float height = dst.w() * uv.w();

	float cx = origin.x() * width;
	float cy = origin.y() * height;

	tgVector2 tl(-cx, -cy);
	tgVector2 tr(width - cx, -cy);
	tgVector2 br(width - cx, height - cy);
	tgVector2 bl(-cx, height - cy);

	tgVector2 pos(dst.x(), dst.y());
	tgVector2 tlr = rotatePoint(tl, rotation) + pos;
	tgVector2 trr = rotatePoint(tr, rotation) + pos;
	tgVector2 brr = rotatePoint(br, rotation) + pos;
	tgVector2 blr = rotatePoint(bl, rotation) + pos;
	
	float u1 = uv.x();
	float v1 = uv.y();
	float u2 = uv.x() + uv.z();
	float v2 = uv.y() + uv.w();

	tgSprite spr;
	spr.TL.position = tgVector3(tlr, 0);
	spr.TL.texco = tgVector2(u1, v1);

	spr.TR.position = tgVector3(trr, 0);
	spr.TR.texco = tgVector2(u2, v1);

	spr.BR.position = tgVector3(brr, 0);
	spr.BR.texco = tgVector2(u2, v2);

	spr.BL.position = tgVector3(blr, 0);
	spr.BL.texco = tgVector2(u1, v2);

	spr.texture = tex->getBindCode();

	m_sprites.push_back(spr);
}
