#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "tgGL.h"
#include "../core/tgUtil.h"

#include "../math/tgMath.h"

#include <unordered_map>
#include <vector>
#include <string>

class tgShaderProgram {
public:

	enum tgShaderType {
		tgVERTEX_SHADER = GL_VERTEX_SHADER,
		tgFRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		tgGEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		tgCOMPUTE_SHADER = GL_COMPUTE_SHADER
	};

	typedef struct tgShader {
		tgShaderType stype;
		int object;
		std::string src;
	} tgShader;

	tgShaderProgram();
	~tgShaderProgram();

	void bind();
	void unbind();

	void addShader(std::string const& src, tgShaderType stype);
	void link();

	bool hasUniform(std::string const& uniform);

	bool isBound() const { return m_bound; }
	bool isValid() const { return m_valid; }

	void setInt(std::string const& name, int value);
	void setFloat(std::string const& name, float value);
	void setFloat2(std::string const& name, tgVector2 const& value);
	void setFloat3(std::string const& name, tgVector3 const& value);
	void setFloat4(std::string const& name, tgVector4 const& value);
	void setMatrix3(std::string const& name, tgMatrix3 const& value);
	void setMatrix4(std::string const& name, tgMatrix4 const& value);
	void setMatrix4Array(std::string const& name, std::vector<tgMatrix4> const& values);

private:
	GLuint m_program;
	std::unordered_map<std::string, GLuint> m_uniforms;
	std::vector<tgShader> m_shaders;
	bool m_valid, m_bound;

	static int createShader(std::string const& src, GLenum stype);

	void addUniform(std::string const& name);
	void addUniformArr(std::string const& name, int n);
};

#endif // SHADER_PROGRAM_H
