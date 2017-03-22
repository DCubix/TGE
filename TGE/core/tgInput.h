#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <SDL2/SDL.h>

typedef struct tgState {
	bool pressed, released, down;
} tgState;


class tgInput {
public:
	static void update();

	static const tgState getKeyboardState(int key);
	static const tgState getMouseState(int button);

	static bool isKeyPressed(int key);
	static bool isKeyReleased(int key);
	static bool isKeyDown(int key);

	static bool isMouseButtonPressed(int btn);
	static bool isMouseButtonReleased(int btn);
	static bool isMouseButtonDown(int btn);

	static int getScrollOffset() { return m_scrollOffset; }
	static int getMouseX() { return m_mouseX; }
	static int getMouseY() { return m_mouseY; }

	static bool isCloseRequested() { return m_closeRequested; }

private:
	static SDL_Event m_sdlEvent;
	static std::unordered_map<int, tgState> m_keyboard;
	static std::unordered_map<int, tgState> m_mouse;
	static int m_mouseX, m_mouseY;
	static int m_scrollOffset;
	static bool m_closeRequested;
};

#endif // INPUT_H
