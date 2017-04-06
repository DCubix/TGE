#include "tgRenderer.h"

#include <sstream>

tgRenderer::tgRenderer(int screen_width, int screen_height) {
	w = screen_width;
	h = screen_height;

	m_projection = tgMatrix4::perspective(45.0f, float(screen_width) / float(screen_height), 0.01f, 100.0f);
	m_view = tgMatrix4::identity();

	m_finalTexture = new tgTexture(screen_width, screen_height, tgTexture::tgTEXTURE_FLOAT3);
	m_finalBuffer = new tgFrameBuffer();
	m_finalBuffer->addColorAttachment(m_finalTexture);
	m_finalTexture->setWrap(GL_CLAMP_TO_EDGE);

	m_pingpong[0] = new tgTexture(screen_width, screen_height, tgTexture::tgTEXTURE_FLOAT3);
	m_pingpong[1] = new tgTexture(screen_width, screen_height, tgTexture::tgTEXTURE_FLOAT3);
	m_pingPongBuffer = new tgFrameBuffer();
	m_pingPongBuffer->addColorAttachment(m_pingpong[0]);
	m_pingPongBuffer->addColorAttachment(m_pingpong[1]);
	m_pingpong[0]->setWrap(GL_CLAMP_TO_EDGE);
	m_pingpong[1]->setWrap(GL_CLAMP_TO_EDGE);

	std::string DEFAULT_VS =
		"#version 440\n"
		"layout (location = 0) in vec3 v_position;"
		"layout (location = 1) in vec2 v_uv;"
		"out vec2 texCoord;"
		"void main() {"
		"	gl_Position = vec4(v_position * 2.0 - 1.0, 1.0);"
		"	texCoord = v_uv;"
		"}";
	std::string DEFAULT_FS =
		"#version 440\n"
		"out vec4 fragColor;"
		"in vec2 texCoord;"
		"uniform sampler2D tex0;"
		"void main() {"
		"	fragColor = texture(tex0, texCoord);"
		"}";
	m_defaultShader = new tgShaderProgram();
	m_defaultShader->addShader(DEFAULT_VS, tgShaderProgram::tgVERTEX_SHADER);
	m_defaultShader->addShader(DEFAULT_FS, tgShaderProgram::tgFRAGMENT_SHADER);
	m_defaultShader->link();
	
	std::string sb_vert =
		"#version 440\n"
		"layout (location = 0) in vec3 v_pos;"
		"layout (location = 1) in vec3 v_nrm;"
		"layout (location = 2) in vec4 v_color;"
		"layout (location = 3) in vec2 v_uv;"
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

	// Main VAO
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(tgVertex), (void*) offsetof(tgVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(tgVertex), (void*) offsetof(tgVertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, true, sizeof(tgVertex), (void*) offsetof(tgVertex, color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(tgVertex), (void*) offsetof(tgVertex, texCoord));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glBindVertexArray(0);

	// Full screen quad VAO
	float quadVerts[] = {
		0, 0, 0, 0, 0,
		1, 0, 0, 1, 0,
		1, 1, 0, 1, 1,
		1, 1, 0, 1, 1,
		0, 1, 0, 0, 1,
		0, 0, 0, 0, 0
	};
	glGenVertexArrays(1, &m_qvao);
	glGenBuffers(1, &m_qvbo);

	glBindVertexArray(m_qvao);

	glBindBuffer(GL_ARRAY_BUFFER, m_qvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 30, quadVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 5, (void*) 12);

	glBindVertexArray(0);

	updateUniforms();
}

tgRenderer::~tgRenderer() {
	if (m_qvao) {
		glDeleteVertexArrays(1, &m_qvao);
	}
	if (m_qvbo) {
		glDeleteBuffers(1, &m_qvbo);
	}
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_vbo) {
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_ibo) {
		glDeleteBuffers(1, &m_ibo);
	}
	tgDelete(m_shader);

	tgDelete(m_pingPongBuffer);
	tgDelete(m_defaultShader);
	tgDelete(m_finalBuffer);
	tgDelete(m_finalTexture);
	for (int i = 0; i < 2; i++) {
		tgDelete(m_pingpong[i]);
	}
	for (tgPostEffect *fx : m_postfx) {
		tgDelete(fx);
	}
	m_postfx.clear();
}

static tgVector2 rotatePoint(tgVector2 const& p, float rad) {
	float c = std::cos(rad);
	float s = std::sin(rad);
	return tgVector2(c * p.x() - s * p.y(), s * p.x() + c * p.y());
}

void tgRenderer::draw(tgTexture * tex) {
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

	spr->tl = TL;
	spr->tr = TR;
	spr->bl = BL;
	spr->br = BR;

	spr->textures[0] = { (int) tex->getBindCode(), tgTextureSlotType::tgCOLOR_TEXTURE };
	spr->blendMode = m_currentState.blendMode;

	submit(spr);
}

void tgRenderer::save() {
	m_prevState = m_currentState;
	m_currentState = tgSpriteBatchState();
	m_currentState.scale = tgVector2(1.0f);
	m_currentState.color = tgVector4(1.0f);
	m_currentState.uv = tgVector4(0, 0, 1, 1);
}

void tgRenderer::restore() {
	m_currentState = m_prevState;
}

void tgRenderer::resize(int w, int h) {
	m_projection = tgMatrix4::ortho(0, float(w), float(h), 0, -100, 100);
	updateUniforms();
}

void tgRenderer::submit(tgSprite *mesh) {
	if (mesh) {
		m_sprites.push_back(mesh);
	}
}

void tgRenderer::updateUniforms() {
	m_shader->bind();
	m_shader->setMatrix4("viewProjection", m_projection * m_view);
	m_shader->setInt("tex0", 0);
}

void tgRenderer::updateBuffers() {
	if (m_sprites.empty()) {
		return;
	}

	std::sort(m_sprites.begin(), m_sprites.end(),
			  [](tgSprite *a, tgSprite *b) -> bool {
		return a->textures[0].id < b->textures[0].id;
	}
	);

	std::vector<tgVertex> vertices;
	vertices.reserve(m_sprites.size() * 4);

	std::vector<int> indices;
	indices.reserve(m_sprites.size() * 6);

	vertices.insert(vertices.end(), { m_sprites[0]->tl, m_sprites[0]->tr, m_sprites[0]->br, m_sprites[0]->bl });
	indices.insert(indices.end(), { 0, 1, 2, 2, 3, 0 });

	m_batches.emplace_back(0, 6, m_sprites[0]->textures, m_sprites[0]->blendMode, m_sprites[0]->tl.position.z());

	int io = 4;
	int off = 0;
	for (std::size_t i = 1; i < m_sprites.size(); ++i) {
		if (m_sprites[i]->textures[0].id != m_sprites[i - 1]->textures[0].id ||
			m_sprites[i]->tl.position.z() != m_sprites[i - 1]->tl.position.z()) {
			off += m_batches.back().numIndices;
			m_batches.emplace_back(off, 6, m_sprites[i]->textures, m_sprites[i]->blendMode, m_sprites[i]->tl.position.z());
		} else {
			m_batches.back().numIndices += 6;
			m_batches.back().blendMode = m_sprites[i]->blendMode;
		}

		vertices.insert(vertices.end(), { m_sprites[i]->tl, m_sprites[i]->tr, m_sprites[i]->br, m_sprites[i]->bl });
		indices.insert(indices.end(), {
			0 + io, 1 + io, 2 + io, 2 + io, 3 + io, 0 + io
		});

		io += 4;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	int vboSize = vertices.size() * sizeof(tgVertex);
	if (vboSize > m_prevVBOSize) {
		glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_DYNAMIC_DRAW);
		m_prevVBOSize = vboSize;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	int iboSize = indices.size() * sizeof(int);
	if (iboSize > m_prevIBOSize) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, nullptr, GL_DYNAMIC_DRAW);
		m_prevIBOSize = iboSize;
	}
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, iboSize, indices.data());
}

void tgRenderer::drawQuad() {
	glBindVertexArray(m_qvao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void tgRenderer::begin() {
	m_shader->bind();
	for (tgSprite *mesh : m_sprites) {
		tgDelete(mesh);
	}
	m_sprites.clear();
	m_batches.clear();
}

void tgRenderer::end() {
	updateBuffers();
}

void tgRenderer::render() {
	beginPostFX();
	renderGeometry(m_shader);
	endPostFX();
}

void tgRenderer::renderGeometry(tgShaderProgram *shader) {
	std::sort(m_batches.begin(), m_batches.end(),
			  [](tgBatch const& a, tgBatch const& b) -> bool {
		return a.z < b.z;
	}
	);
	
	glBindVertexArray(m_vao);
	for (tgBatch &batch : m_batches) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, batch.textures[0].id); // TODO: Multitexture
		m_shader->setInt("tex0", 0);

		switch (batch.blendMode) {
			case tgBLEND_NORMAL: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
			case tgBLEND_ADD: glBlendFunc(GL_ONE, GL_ONE); break;
		}

		glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, (void*) (batch.offset * sizeof(int)));
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindVertexArray(0);
	shader->unbind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void tgRenderer::beginPostFX() {
	m_finalBuffer->bind();
	clear(GL_COLOR_BUFFER_BIT);
}

void tgRenderer::postProcess() {
	if (!m_postfx.empty()) {
		m_pingPongBuffer->bind();
		clear(GL_COLOR_BUFFER_BIT);

		int src = 0, prevSrc = 0;
		bool isFirst = true;
		for (tgPostEffect *fx : m_postfx) {
			for (int i = 0; i < fx->getRepeatCount(); i++) {
				glDrawBuffer(GL_COLOR_ATTACHMENT0 + src);

				fx->bind();
				if (isFirst) {
					m_finalTexture->bind(0);
				} else {
					m_pingpong[prevSrc]->generateMipMaps();
					m_pingpong[prevSrc]->bind(0);
				}
				fx->setInt("tex0", 0);

				if (fx->hasUniform("resolution")) {
					fx->setFloat2("resolution", tgVector2(w, h));
				}

				fx->update();

				drawQuad();

				fx->unbind();

				if (isFirst) {
					isFirst = false;
					m_finalTexture->unbind();
				} else {
					m_pingpong[prevSrc]->unbind();
				}
				prevSrc = src;
				src = 1 - prevSrc;
			}
		}
		m_pingPongBuffer->unbind();
		glViewport(0, 0, w, h);

		m_defaultShader->bind();
		m_pingpong[prevSrc]->bind(0);
		m_defaultShader->setInt("tex0", 0);

		drawQuad();

		m_pingpong[prevSrc]->unbind();
		m_defaultShader->unbind();
	} else {
		basicPostProcess();
	}
}

void tgRenderer::endPostFX() {
	m_finalBuffer->unbind();
	m_finalTexture->generateMipMaps();
}

void tgRenderer::basicPostProcess() {
	glViewport(0, 0, w, h);

	m_defaultShader->bind();
	m_finalTexture->bind(0);
	m_defaultShader->setInt("tex0", 0);

	drawQuad();

	m_finalTexture->unbind();
	m_defaultShader->unbind();
}

void tgRenderer::addPostEffect(tgPostEffect *fx) {
	if (fx) {
		m_postfx.push_back(fx);
	}
}

void tgRenderer::removePostEffect(int passIndex) {
	tgPostEffect *fx = m_postfx[passIndex];
	m_postfx[passIndex] = m_postfx.back();
	m_postfx.pop_back();
	tgDelete(fx);
}

void tgRenderer::clear(int flags) {
	glClear(flags);
}

void tgRenderer::setClearColor(tgVector4 const& color) {
	glClearColor(color.x(), color.y(), color.z(), color.w());
}
