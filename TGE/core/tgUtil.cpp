#include "tgUtil.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <AL/al.h>

#include "tgLog.h"

static const char* errorCheck(ALenum error) {
	if (error == AL_INVALID_NAME) {
		return "Invalid name";
	} else if (error == AL_INVALID_ENUM) {
		return "Invalid enum";
	} else if (error == AL_INVALID_VALUE) {
		return "Invalid value";
	} else if (error == AL_INVALID_OPERATION) {
		return "Invalid operation";
	} else if (error == AL_OUT_OF_MEMORY) {
		return "Out of memory like!";
	}
	return "unknown";
}

unsigned int checkError() {
	return glGetError();
}

unsigned int checkALError() {
	return alGetError();
}

bool glLog(const char* function, const char* file, int line) {
	unsigned int error = checkError();
	if(error != GL_NO_ERROR) {
		tgLog::log("OpenGL Error (", error, "): ", function, "\n\t", file, ":", line, ":\n\t\t", gluErrorString(error));
		return false;
	}
	return true;
}

void alLog(const char * function, const char * file, int line) {
	ALuint err = alGetError();
	if (err != AL_NO_ERROR) {
		tgLog::log("OpenAL Error (", err, "): ", function, "\n\t", file, ":", line, ":\n\t\t", errorCheck(err));
	}
}
