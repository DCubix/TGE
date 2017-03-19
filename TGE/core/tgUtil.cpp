#include "tgUtil.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include "tgLog.h"

unsigned int checkError() {
	return glGetError();
}

bool glLog(const char* function, const char* file, int line) {
	unsigned int error = checkError();
	if(error != GL_NO_ERROR) {
		tgLog::log("OpenGL Error (", error, "): ", function, "\n\t", file, ":", line, ":\n\t\t", gluErrorString(error));
		return false;
	}
	return true;
}
