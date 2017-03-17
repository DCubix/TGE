#include "tgShaderProgram.h"

#include <sstream>

tgShaderProgram::tgShaderProgram()
	: m_program(-1),
	m_valid(true),
	m_bound(false) {
	m_program = glCreateProgram();
}

tgShaderProgram::~tgShaderProgram() {}

int tgShaderProgram::createShader(std::string const& src, GLenum stype) {
	int shader = glCreateShader(stype);
	const char *c_str = src.c_str();
	GLCall(glShaderSource(shader, 1, (const GLchar**) &c_str, nullptr));
	GLCall(glCompileShader(shader));

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		char log[1024];
		glGetShaderInfoLog(shader, 1024, nullptr, log);
		Err(log);
		return -1;
	}

	return shader;
}

void tgShaderProgram::bind() {
	if(isValid()) {
		m_bound = true;
		GLCall(glUseProgram(m_program));
	}
}

void tgShaderProgram::unbind() {
	if(isBound()) {
		m_bound = false;
		GLCall(glUseProgram(0));
	}
}

void tgShaderProgram::addShader(std::string const& src, tgShaderType stype) {
	if(src.empty()) {
		Err("Your shader source cannot be empty.");
		m_valid = false;
	}

	if(!m_valid) {
		return;
	}

	int shader = createShader(src, (GLenum) stype);
	if(shader != -1) {
		for(tgShader s : m_shaders) {
			if(s.stype == stype) {
				Err("You can't have more than 1 shader of this type.");
				m_valid = false;
				break;
			}
		}
		if(m_valid) {
			GLCall(glAttachShader(m_program, shader));
			m_shaders.push_back({ stype, shader, src });
		}
	} else {
		Err("Could not create shader.");
		m_valid = false;
	}
}

void tgShaderProgram::link() {
	if(!m_valid) {
		Err("Cannot link an invalid shader program.");
		return;
	}

	GLCall(glLinkProgram(m_program));
	int status;
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		m_valid = false;
		char log[1024];
		glGetProgramInfoLog(m_program, 1024, nullptr, log);
		Err(log);
	} else {
		int uniforms;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniforms);
		for(int i = 0; i < uniforms; i++) {
			int size, len;
			GLenum type_;
			char name[256];
			glGetActiveUniform(m_program, i, 256, &len, &size, &type_, name);
			if(size != 1) {
				std::string name_(name);
				name_ = name_.substr(0, name_.find("["));
				addUniformArr(name_, size);
			} else {
				addUniform(std::string(name));
			}
		}

		for(tgShader shad : m_shaders) {
			glDeleteShader(shad.object);
		}
		m_shaders.clear();
	}
}

bool tgShaderProgram::hasUniform(std::string const& uniform) {
	auto pos = m_uniforms.find(uniform);
	return pos != m_uniforms.end();
}

void tgShaderProgram::addUniform(std::string const& name) {
	int loc = glGetUniformLocation(m_program, name.c_str());
	if(loc != -1) {
		m_uniforms[name] = loc;
	} else {
		Err("Could not find an uniform named \"" << name << "\".");
	}
}

void tgShaderProgram::addUniformArr(std::string const& name, int n) {
	for(int i = 0; i < n; i++) {
		std::stringstream stm;
		stm << name << "[" << i << "]";
		addUniform(stm.str());
	}
}

void tgShaderProgram::setInt(std::string const& name, int value) {
	if(hasUniform(name)) {
		GLCall(glUniform1i(m_uniforms[name], value));
	}
}

void tgShaderProgram::setFloat(std::string const& name, float value) {
	if(hasUniform(name)) {
		GLCall(glUniform1f(m_uniforms[name], value));
	}
}

void tgShaderProgram::setFloat2(std::string const& name, tgVector2 const& value) {
	if(hasUniform(name)) {
		GLCall(glUniform2f(m_uniforms[name], value[0], value[1]));
	}
}

void tgShaderProgram::setFloat3(std::string const& name, tgVector3 const& value) {
	if(hasUniform(name)) {
		GLCall(glUniform3f(m_uniforms[name], value[0], value[1], value[2]));
	}
}

void tgShaderProgram::setFloat4(std::string const& name, tgVector4 const& value) {
	if(hasUniform(name)) {
		GLCall(glUniform4f(m_uniforms[name], value[0], value[1], value[2], value[3]));
	}
}

void tgShaderProgram::setMatrix3(std::string const& name, tgMatrix3 const& value) {
	if(hasUniform(name)) {
		GLCall(glUniformMatrix3fv(m_uniforms[name], 1, true, &value[0][0]));
	}
}

void tgShaderProgram::setMatrix4(std::string const& name, tgMatrix4 const& value) {
	if(hasUniform(name)) {
		GLCall(glUniformMatrix4fv(m_uniforms[name], 1, true, &value[0][0]));
	}
}

void tgShaderProgram::setMatrix4Array(std::string const& name, std::vector<tgMatrix4> const& values) {
	if(hasUniform(name)) {
		std::vector<float> val;
		val.reserve(values.size() * 16);
		for(tgMatrix4 mat : values) {
			for(unsigned i = 0; i < 4; i++) {
				for(unsigned j = 0; j < 4; j++) {
					val.push_back(mat[i][j]);
				}
			}
		}
		GLCall(glUniformMatrix4fv(m_uniforms[name], values.size(), false, &val[0]));
	}
}
