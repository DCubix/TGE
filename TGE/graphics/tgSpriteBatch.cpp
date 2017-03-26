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
	fmt.append(tgVertexFormat::tgATTR_FLOAT2, false);
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
		"layout (location = 0) in vec2 v_pos;"
		"layout (location = 1) in vec2 v_uv;"
		"layout (location = 2) in vec4 v_color;"
		"out DATA {"
		"	vec2 position;"
		"	vec2 uv;"
		"	vec4 color;"
		"} vs_out;"
		"uniform mat4 viewProjection = mat4(1.0);"
		"void main() {"
		"	gl_Position = viewProjection * vec4(v_pos, 0.0, 1.0);"
		"	vs_out.position = v_pos;"
		"	vs_out.uv = v_uv;"
		"	vs_out.color = v_color;"
		"}";
	std::string sb_frag =
		"#version 440\n"
		"out vec4 fragColor;"
		"in DATA {"
		"	vec2 position;"
		"	vec2 uv;"
		"	vec4 color;"
		"} fs_in;"
		"uniform sampler2D tex0;"
		"void main() {"
		"	vec4 col = texture(tex0, fs_in.uv);"
		"	fragColor = col * fs_in.color;"
		"}";
	m_shader = new tgShaderProgram();
	m_shader->addShader(sb_vert, tgShaderProgram::tgVERTEX_SHADER);
	m_shader->addShader(sb_frag, tgShaderProgram::tgFRAGMENT_SHADER);
	m_shader->link();

	m_projection = tgMatrix4::ortho(0, float(screen_width), float(screen_height), 0, -100, 100);
	m_view = tgMatrix4::identity();

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_currentState = tgSpriteBatchState();
	m_currentState.scale = tgVector2(1.0f);
	m_currentState.color = tgVector4(1.0f);
	m_currentState.uv = tgVector4(0, 0, 1, 1);

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

void tgSpriteBatch::draw(tgTexture *tex) {
	if (!tex) { return; }

	float z = m_currentState.position.z();
	tgVector4 dst = tgVector4(
		m_currentState.position.x(),
		m_currentState.position.y(),
		m_currentState.scale.x() * float(tex->getWidth()),
		m_currentState.scale.y() * float(tex->getHeight())
	);
	tgVector4 uv = m_currentState.uv;
	tgVector2 origin = m_currentState.origin;
	tgVector4 color = m_currentState.color;
	float rotation = m_currentState.rotation;

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
	spr->TL.position = tlr;
	spr->TL.texco = tgVector2(u1, v1);
	spr->TL.color = color;

	spr->TR.position = trr;
	spr->TR.texco = tgVector2(u2, v1);
	spr->TR.color = color;

	spr->BR.position = brr;
	spr->BR.texco = tgVector2(u2, v2);
	spr->BR.color = color;

	spr->BL.position = blr;
	spr->BL.texco = tgVector2(u1, v2);
	spr->BL.color = color;

	spr->texture = tex->getBindCode();
	spr->blend = m_currentState.blendMode;
	spr->z = z;

	m_sprites.push_back(spr);
}

void tgSpriteBatch::save() {
	m_prevState = m_currentState;
	m_currentState = tgSpriteBatchState();
	m_currentState.scale = tgVector2(1.0f);
	m_currentState.color = tgVector4(1.0f);
	m_currentState.uv = tgVector4(0, 0, 1, 1);
}

void tgSpriteBatch::restore() {
	m_currentState = m_prevState;
}

void tgSpriteBatch::resize(int w, int h) {
	m_projection = tgMatrix4::ortho(0, float(w), float(h), 0, -100, 100);
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

	m_batches.emplace_back(0, 6, m_sprites[0]->texture, m_sprites[0]->blend, m_sprites[0]->z);

	int indexOffset = 4;
	int off = 0;
	for (std::size_t i = 1; i < m_sprites.size(); ++i) {
		if (m_sprites[i]->texture != m_sprites[i - 1]->texture ||
			m_sprites[i]->z != m_sprites[i - 1]->z) {
			off += m_batches.back().numIndices;
			m_batches.emplace_back(off, 6,  m_sprites[i]->texture, m_sprites[i]->blend, m_sprites[i]->z);
		} else {
			m_batches.back().numIndices += 6;
			m_batches.back().blend = m_sprites[i]->blend;
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
	std::sort(m_batches.begin(), m_batches.end(),
		[](tgBatch const& a, tgBatch const& b) -> bool {
		return a.z < b.z;
	}
	);
	m_vao->bind();
	for (tgBatch &batch : m_batches) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, batch.texture);
		m_shader->setInt("tex0", 0);

		switch (batch.blend) {
			case tgBLEND_NORMAL: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
			case tgBLEND_ADD: glBlendFunc(GL_ONE, GL_ONE); break;
		}

		m_vao->drawElements(tgVertexArrayObject::tgPRIM_TRIANGLES, batch.numIndices, batch.offset);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	m_vao->unbind();
}
