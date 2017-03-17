#include "tgUtil.h"

#include <GL/glew.h>
#include <GL/glu.h>

unsigned int checkError() {
	return glGetError();
}

bool glLog(const char* function, const char* file, int line) {
	unsigned int error = checkError();
	if(error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error (" << error << "): " << function << "\n\t"
			<< file << ":" << line << ":\n\t\t" << gluErrorString(error) << std::endl;
		return false;
	}
	return true;
}
