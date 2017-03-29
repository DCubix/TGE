#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <functional>
#include <SDL2/SDL.h>

enum tgOpenGLContextProfileMask {
	tgOGL_CORE_PROFILE = SDL_GL_CONTEXT_PROFILE_CORE,
	tgOGL_COMPAT_PROFILE = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
	tgOGL_ES_PROFILE = SDL_GL_CONTEXT_PROFILE_ES
};

typedef struct tgOpenGLConfiguration {
	int maj_version, min_version;
	int aa_samples;
	int red_size;
	int green_size;
	int blue_size;
	int alpha_size;
	int depth_size;
	int stencil_size;
	tgOpenGLContextProfileMask profile;
} tgOpenGLConfiguration;
#define DEFAULT_OGL_CONFIG { 4, 5, 0, 8, 8, 8, 8, 16, 8, tgOGL_CORE_PROFILE }

class tgWindow {
public:
	tgWindow(std::string const &title, int width, int height, tgOpenGLConfiguration ogl=DEFAULT_OGL_CONFIG);
	~tgWindow();

	void setTitle(std::string const &title) { m_title = title; }

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	const std::string& getTitle() const { return m_title; }

	bool shouldClose() const { return m_shouldClose; }
	void close() { m_shouldClose = true; }
	void setFullScreen(bool enable);
	void swapBuffers();
	void show();
	void hide();

private:
	std::string m_title;
	int m_width, m_height;
	bool m_shouldClose;

	SDL_Window *m_sdlWindow;
	SDL_GLContext m_sdl_glContext;
};

#endif // WINDOW_H
