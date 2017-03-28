#include "tgWindow.h"
#include "tgUtil.h"

#include "../graphics/tgGL.h"
#include "tgLog.h"

tgWindow::tgWindow(std::string const &title, int width, int height, tgOpenGLConfiguration ogl)
	: m_title(title), m_width(width), m_height(height), m_sdlWindow(nullptr)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		tgLog::log("Could not initialize SDL backend. ", SDL_GetError());
	} else {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ogl.maj_version);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ogl.min_version);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, ogl.profile);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, ogl.red_size);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, ogl.green_size);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, ogl.blue_size);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, ogl.alpha_size);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, ogl.depth_size);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, ogl.stencil_size);

		if (ogl.aa_samples > 0) {
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ogl.aa_samples);
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_sdlWindow = SDL_CreateWindow(m_title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_width,
			m_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);
		if (m_sdlWindow == nullptr) {
			tgLog::log("Could not create a window. ", SDL_GetError());
		}

		m_sdl_glContext = SDL_GL_CreateContext(m_sdlWindow);
		if (m_sdl_glContext == nullptr) {
			tgLog::log("Could not create an OpenGL context. ", SDL_GetError());
		}
		SDL_GL_MakeCurrent(m_sdlWindow, m_sdl_glContext);

		glewExperimental = GL_TRUE;
		int err = glewInit();
		if (err != GLEW_OK) {
			tgLog::log("Could not initialize OpenGL extensions. ", glewGetErrorString(err));
		}
	}
}

tgWindow::~tgWindow() {
	if(m_sdlWindow) {
		SDL_GL_DeleteContext(m_sdl_glContext);
		SDL_DestroyWindow(m_sdlWindow);
		m_sdlWindow = nullptr;
	}
}

void tgWindow::setFullScreen(bool enable) {
	if (enable) {
		SDL_SetWindowFullscreen(m_sdlWindow, SDL_WINDOW_FULLSCREEN);
	} else {
		SDL_SetWindowFullscreen(m_sdlWindow, 0);
	}
}

void tgWindow::swapBuffers() {
	if(m_sdlWindow) {
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}
