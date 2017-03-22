#include "tgSpriteBatch.h"

tgSpriteBatch::tgSpriteBatch(int screen_width, int screen_height)
	: m_vbo(nullptr),
	m_ibo(nullptr),
	m_vao(nullptr),
	m_prevVBOSize(0),
	m_prevIBOSize(0),
	m_drawing(false)
{
	tgVertexFormat fmt(sizeof(tgVertex2D));
	fmt.append(tgVertexFormat::tgATTR_POSITION, false);
	fmt.append(tgVertexFormat::tgATTR_TEXCOORD, false);
	fmt.append(tgVertexFormat::tgATTR_COLOR, true);

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
		"layout (location = 2) in vec4 v_color;"
		"out DATA {"
		"	vec3 position;"
		"	vec2 uv;"
		"	vec4 color;"
		"} vs_out;"
		"uniform mat4 viewProjection = mat4(1.0);"
		"void main() {"
		"	gl_Position = viewProjection * vec4(v_pos, 1.0);"
		"	vs_out.position = v_pos;"
		"	vs_out.uv = v_uv;"
		"	vs_out.color = v_color;"
		"}";
	std::string sb_frag =
		"#version 440\n"
		"out vec4 fragColor;"
		"in DATA {"
		"	vec3 position;"
		"	vec2 uv;"
		"	vec4 color;"
		"} fs_in;"
		"uniform sampler2D tex0;"
		"void main() {"
		"	fragColor = texture(tex0, fs_in.uv) * fs_in.color;"
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

tgSpriteBatch::~tgSpriteBatch() {
	tgDelete(m_ibo);
	tgDelete(m_vbo);
	tgDelete(m_vao);
	tgDelete(m_shader);
}

static tgVector2 rotatePoint(tgVector2 const& p, float rad) {
	float c = std::cos(rad);
	float s = std::sin(rad);
	return tgVector2(c * p.x() - s * p.y(), s * p.x() + c * p.y());
}

void tgSpriteBatch::draw(tgTexture * tex, tgVector4 const & uv, tgVector4 const & dst, tgVector2 const & origin, float rotation, tgVector4 const & color) {
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

	tgSprite *spr = new tgSprite();
	spr->TL.position = tgVector3(tlr, 0);
	spr->TL.texco = tgVector2(u1, v1);
	spr->TL.color = color;

	spr->TR.position = tgVector3(trr, 0);
	spr->TR.texco = tgVector2(u2, v1);
	spr->TR.color = color;

	spr->BR.position = tgVector3(brr, 0);
	spr->BR.texco = tgVector2(u2, v2);
	spr->BR.color = color;

	spr->BL.position = tgVector3(blr, 0);
	spr->BL.texco = tgVector2(u1, v2);
	spr->BL.color = color;

	spr->texture = tex->getBindCode();

	m_sprites.push_back(spr);
}

void tgSpriteBatch::drawTile(tgTexture * atlas, int tileIndex, int tileWidth, int tileHeight, int tileX, int tileY, float scale) {
	int cols = atlas->getWidth() / tileWidth;
	int rows = atlas->getHeight() / tileHeight;

	float tx = tileX * float(tileWidth) * scale;
	float ty = tileY * float(tileHeight) * scale;

	float uvw = 1.0f / float(cols);
	float uvh = 1.0f / float(rows);
	float uvx = float(tileIndex % cols) * uvw;
	float uvy = float(int(tileIndex / cols)) * uvh;

	tgVector4 uv(uvx, uvy, uvw, uvh);
	draw(atlas, uv, tgVector4(tx, ty, atlas->getWidth() * scale, atlas->getHeight() * scale), tgVector2(0.0f), 0.0f, tgVector4(1.0f));
}

void tgSpriteBatch::resize(int w, int h) {
	m_projection = tgMatrix4::ortho(0, float(w), float(h), 0, -1, 1);
	updateUniforms();
}

void tgSpriteBatch::begin() {
	if (!m_drawing) {
		m_drawing = true;
		m_shader->bind();

		for (tgSprite *spr : m_sprites) {
			tgDelete(spr);
		}
		m_sprites.clear();

		m_batches.clear();
	}
}

void tgSpriteBatch::end() {
	if (m_drawing) {
		m_drawing = false;
		updateBuffers();
		render();
		m_shader->unbind();
	}
}

void tgSpriteBatch::updateUniforms() {
	m_shader->bind();
	m_shader->setMatrix4("viewProjection", m_projection * m_view);
	m_shader->setInt("tex0", 0);
}

void tgSpriteBatch::updateBuffers() {
	if (m_sprites.size() == 0) {
		return;
	}

	std::sort(m_sprites.begin(), m_sprites.end(),
		[](tgSprite const* a, tgSprite const* b) -> bool {
		return a->texture < b->texture;
	}
	);

	std::vector<tgVertex2D> vertices;
	vertices.reserve(m_sprites.size() * 4);

	std::vector<int> indices;
	indices.reserve(m_sprites.size() * 6);

	vertices.insert(vertices.end(),
	{ m_sprites[0]->BL, m_sprites[0]->TL, m_sprites[0]->TR, m_sprites[0]->BR }
	);

	indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });

	m_batches.emplace_back(0, 6, m_sprites[0]->texture);

	int indexOffset = 4;
	int off = 0;
	for (std::size_t i = 1; i < m_sprites.size(); ++i) {
		if (m_sprites[i]->texture != m_sprites[i - 1]->texture) {
			off += m_batches.back().numIndices;
			m_batches.push_back({ off, 6,  m_sprites[i]->texture });
		} else {
			m_batches.back().numIndices += 6;
		}

		vertices.insert(vertices.end(),
		{ m_sprites[i]->BL, m_sprites[i]->TL, m_sprites[i]->TR, m_sprites[i]->BR }
		);

		indices.insert(indices.end(),
		{ 0 + indexOffset, 1 + indexOffset, 2 + indexOffset,
			2 + indexOffset, 3 + indexOffset, 0 + indexOffset }
		);

		indexOffset += 4;
	}

	m_vbo->bind();
	int vboSize = vertices.size() * sizeof(tgVertex2D);
	if (vboSize > m_prevVBOSize) {
		m_vbo->resize(vboSize);
		m_prevVBOSize = vboSize;
	}
	m_vbo->update(0, vertices.data(), vboSize);

	m_ibo->bind();
	int iboSize = indices.size() * sizeof(int);
	if (iboSize > m_prevIBOSize) {
		m_ibo->resize(iboSize);
		m_prevIBOSize = iboSize;
	}
	m_ibo->update(0, indices.data(), iboSize);
}

void tgSpriteBatch::render() {
	m_vao->bind();
	for (tgBatch &batch : m_batches) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, batch.texture);
		m_shader->setInt("tex0", 0);

		m_vao->drawElements(tgVertexArrayObject::tgPRIM_TRIANGLES, batch.numIndices, batch.offset);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	m_vao->unbind();
}
