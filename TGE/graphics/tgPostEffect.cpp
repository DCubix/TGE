#include "tgPostEffect.h"

tgPostEffect::tgPostEffect()
	: tgShaderProgram(), m_repeatCount(1), m_set(false)
{
	std::string DEFAULT_VS =
		"#version 440\n"
		"layout (location = 0) in vec3 v_position;"
		"layout (location = 1) in vec2 v_uv;"
		"out vec2 texCoord;"
		"void main() {"
		"	gl_Position = vec4(v_position * 2.0 - 1.0, 1.0);"
		"	gl_Position.z = 0.0;"
		"	texCoord = v_uv;"
		"}";
	addShader(DEFAULT_VS, tgShaderProgram::tgVERTEX_SHADER);
}

void tgPostEffect::setShader(std::string const& src) {
	if (!m_set) {
		addShader(src, tgShaderProgram::tgFRAGMENT_SHADER);
		link();
		m_set = true;
	}
}
