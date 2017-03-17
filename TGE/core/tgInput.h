#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <SDL2/SDL.h>

typedef struct tgState {
	bool pressed, released, down;
} tgState;


class tgInput {
public:
	tgInput();

	void update();

	const tgState getKeyboardState(int key);
	const tgState getMouseState(int button);

	bool isKeyPressed(int key);
	bool isKeyReleased(int key);
	bool isKeyDown(int key);

	bool isMouseButtonPressed(int btn);
	bool isMouseButtonReleased(int btn);
	bool isMouseButtonDown(int btn);

	int getScrollOffset() const { return m_scrollOffset; }
	int getMouseX() const { return m_mouseX; }
	int getMouseY() const { return m_mouseY; }

	bool isCloseRequested() const { return m_closeRequested; }

private:
	SDL_Event m_sdlEvent;
	std::unordered_map<int, tgState> m_keyboard;
	std::unordered_map<int, tgState> m_mouse;
	int m_mouseX, m_mouseY;
	int m_scrollOffset;
	bool m_closeRequested;
};

#endif // INPUT_H
