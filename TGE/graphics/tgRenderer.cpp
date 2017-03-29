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

	tgVertexFormat fmt(5 * sizeof(float));
	fmt.append(tgVertexFormat::tgATTR_POSITION, false);
	fmt.append(tgVertexFormat::tgATTR_TEXCOORD, false);

	m_qvbo = new tgBuffer(tgBuffer::tgTARG_ARRAY_BUFFER, tgBuffer::tgUSAGE_STATIC);
	m_qvao = new tgVertexArrayObject();
	m_qvao->bind();
	m_qvbo->bind();
	fmt.bind();
	m_qvao->unbind();

	float quadVerts[] = {
		0, 0, 0, 0, 0,
		1, 0, 0, 1, 0,
		1, 1, 0, 1, 1,
		1, 1, 0, 1, 1,
		0, 1, 0, 0, 1,
		0, 0, 0, 0, 0
	};
	m_qvbo->bind();
	m_qvbo->setData(quadVerts, sizeof(float) * 30);
}

tgRenderer::~tgRenderer() {
	tgDelete(m_qvbo);
	tgDelete(m_qvao);
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

void tgRenderer::submit(tgMesh *mesh) {
	if (mesh) {
		m_meshes.push_back(mesh);
	}
}

void tgRenderer::begin() {
	for (tgMesh *mesh : m_meshes) {
		tgDelete(mesh);
	}
	m_meshes.clear();
}

void tgRenderer::beginPostFX() {
	m_finalBuffer->bind();
	clear(GL_COLOR_BUFFER_BIT);
}

void tgRenderer::renderPostFX() {
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

				m_qvao->bind();
				m_qvao->drawArrays(tgVertexArrayObject::tgPRIM_TRIANGLES, 0, 6);
				m_qvao->unbind();

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

		m_qvao->bind();
		m_qvao->drawArrays(tgVertexArrayObject::tgPRIM_TRIANGLES, 0, 6);
		m_qvao->unbind();

		m_pingpong[prevSrc]->unbind();
		m_defaultShader->unbind();
	} else {
		glViewport(0, 0, w, h);

		m_defaultShader->bind();
		m_finalTexture->bind(0);
		m_defaultShader->setInt("tex0", 0);

		m_qvao->bind();
		m_qvao->drawArrays(tgVertexArrayObject::tgPRIM_TRIANGLES, 0, 6);
		m_qvao->unbind();

		m_finalTexture->unbind();
		m_defaultShader->unbind();
	}
}

void tgRenderer::endPostFX() {
	m_finalBuffer->unbind();
	m_finalTexture->generateMipMaps();
}

void tgRenderer::addPostEffect(tgPostEffect *fx) {
	if (fx) {
		m_postfx.push_back(fx);
	}
}

void tgRenderer::removePostEffect(int passIndex) {
	tgDelete(m_postfx[passIndex]);
	m_postfx[passIndex] = m_postfx.back();
	m_postfx.pop_back();
}

void tgRenderer::clear(int flags) {
	glClear(flags);
}
