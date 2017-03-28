#include "tgSpriteBatch.h"

tgSpriteBatch::tgSpriteBatch(int screen_width, int screen_height)
	: m_vbo(nullptr),
	m_ibo(nullptr),
	m_vao(nullptr),
	m_prevVBOSize(0),
	m_prevIBOSize(0),
	m_drawing(false)
{
	tgVertexFormat fmt(sizeof(tgVertex));
	fmt.append(tgVertexFormat::tgATTR_POSITION, false);
	fmt.append(tgVertexFormat::tgATTR_NORMAL, false);
	fmt.append(tgVertexFormat::tgATTR_TANGENT, false);
	fmt.append(tgVertexFormat::tgATTR_COLOR, true);
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
		"layout (location = 1) in vec3 v_nrm;"
		"layout (location = 2) in vec3 v_tan;"
		"layout (location = 3) in vec4 v_color;"
		"layout (location = 4) in vec2 v_uv;"
		"out DATA {"
		"	vec2 position;"
		"	vec2 uv;"
		"	vec4 color;"
		"} vs_out;"
		"uniform mat4 viewProjection = mat4(1.0);"
		"void main() {"
		"	gl_Position = viewProjection * vec4(v_pos,  1.0);"
		"	vs_out.position = v_pos.xy;"
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

	tgMesh *msh = new tgMesh();

	tgVertex TL;
	TL.position = tgVector3(tlr, z);
	TL.texCoord = tgVector2(u1, v1);
	TL.color = color;

	tgVertex TR;
	TR.position = tgVector3(trr, z);
	TR.texCoord = tgVector2(u2, v1);
	TR.color = color;

	tgVertex BR;
	BR.position = tgVector3(brr, z);
	BR.texCoord = tgVector2(u2, v2);
	BR.color = color;

	tgVertex BL;
	BL.position = tgVector3(blr, z);
	BL.texCoord = tgVector2(u1, v2);
	BL.color = color;

	msh->vertices.insert(msh->vertices.end(), { TL, TR, BR, BL });
	msh->indices.insert(msh->indices.end(), { 0, 1, 2, 2, 3, 0 });

	msh->texture = { (int)tex->getBindCode(), tgTextureSlotType::tgALBEDO_TEXTURE };
	msh->blendMode = m_currentState.blendMode;
	msh->modelMatrix = tgMatrix4::identity();

	submit(msh);
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
	m_shader->bind();

	tgRenderer::begin();

	m_batches.clear();
}

void tgSpriteBatch::end() {
	updateBuffers();
}

void tgSpriteBatch::updateUniforms() {
	m_shader->bind();
	m_shader->setMatrix4("viewProjection", m_projection * m_view);
	m_shader->setInt("tex0", 0);
}

void tgSpriteBatch::updateBuffers() {
	if (m_meshes.empty()) {
		return;
	}

	std::sort(m_meshes.begin(), m_meshes.end(),
		[](tgMesh *a, tgMesh *b) -> bool {
		return a->texture.id < b->texture.id;
	}
	);

	std::vector<tgVertex> vertices;
	vertices.reserve(m_meshes.size() * 4);

	std::vector<int> indices;
	indices.reserve(m_meshes.size() * 6);

	vertices.insert(vertices.end(), m_meshes[0]->vertices.begin(), m_meshes[0]->vertices.end());
	indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });

	m_batches.emplace_back(0, 6, m_meshes[0]->texture.id, m_meshes[0]->blendMode, m_meshes[0]->vertices[0].position.z());

	int indexOffset = 4;
	int off = 0;
	for (std::size_t i = 1; i < m_meshes.size(); ++i) {
		if (m_meshes[i]->texture.id != m_meshes[i - 1]->texture.id ||
			m_meshes[i]->vertices[0].position.z() != m_meshes[i - 1]->vertices[0].position.z()) {
			off += m_batches.back().numIndices;
			m_batches.emplace_back(off, 6, m_meshes[i]->texture.id, m_meshes[i]->blendMode, m_meshes[i]->vertices[0].position.z());
		} else {
			m_batches.back().numIndices += 6;
			m_batches.back().blend = m_meshes[i]->blendMode;
		}

		vertices.insert(vertices.end(), m_meshes[i]->vertices.begin(), m_meshes[i]->vertices.end());
		indices.insert(indices.end(), {
			0 + indexOffset, 1 + indexOffset, 2 + indexOffset, 2 + indexOffset, 3 + indexOffset, 0 + indexOffset
		});

		indexOffset += 4;
	}

	m_vbo->bind();
	int vboSize = vertices.size() * sizeof(tgVertex);
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
	renderGeometry(m_shader);
}

void tgSpriteBatch::renderGeometry(tgShaderProgram *shader) {
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
	shader->unbind();
}
