#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL2/SDL.h>

class tgWindow {
public:
	tgWindow(std::string const &title, int width, int height);
	virtual ~tgWindow();

	void setTitle(std::string const &title) { m_title = title; }

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	const std::string& getTitle() const { return m_title; }

	bool shouldClose() const { return m_shouldClose; }
	void close() { m_shouldClose = true; }

	void swapBuffers();

private:
	std::string m_title;
	int m_width, m_height;
	bool m_shouldClose;

	SDL_Window *m_sdlWindow;
	SDL_GLContext m_sdl_glContext;
};

#endif // WINDOW_H
