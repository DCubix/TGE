#include "tgInput.h"
#include "tgUtil.h"

#include "tgLog.h"

SDL_Event tgInput::m_sdlEvent;
std::unordered_map<int, tgState> tgInput::m_keyboard;
std::unordered_map<int, tgState> tgInput::m_mouse;
int tgInput::m_mouseX = 0;
int tgInput::m_mouseY = 0;
int tgInput::m_scrollOffset = 0;
bool tgInput::m_closeRequested = false;

void tgInput::update() {
	for(auto& kv : m_keyboard) {
		kv.second.pressed = false;
		kv.second.released = false;
	}
	for(auto& kv : m_mouse) {
		kv.second.pressed = false;
		kv.second.released = false;
	}

	while(SDL_PollEvent(&m_sdlEvent)) {
		switch(m_sdlEvent.type) {
			case SDL_QUIT: m_closeRequested = true; break;
			case SDL_KEYDOWN:
			{
				int key = m_sdlEvent.key.keysym.sym;
				m_keyboard[key].pressed = true;
				m_keyboard[key].down = true;
			} break;
			case SDL_KEYUP:
			{
				int key = m_sdlEvent.key.keysym.sym;
				m_keyboard[key].released = true;
				m_keyboard[key].down = false;
			} break;
			case SDL_MOUSEBUTTONDOWN:
			{
				int btn = m_sdlEvent.button.button;
				m_mouse[btn].pressed = true;
				m_mouse[btn].down = true;
				m_mouseX = m_sdlEvent.button.x;
				m_mouseY = m_sdlEvent.button.y;
			} break;
			case SDL_MOUSEBUTTONUP:
			{
				int btn = m_sdlEvent.button.button;
				m_mouse[btn].released = true;
				m_mouse[btn].down = false;
				m_mouseX = m_sdlEvent.button.x;
				m_mouseY = m_sdlEvent.button.y;
			} break;
			case SDL_MOUSEMOTION:
			{
				m_mouseX = m_sdlEvent.motion.x;
				m_mouseY = m_sdlEvent.motion.y;
			} break;
			case SDL_MOUSEWHEEL:
			{
				m_scrollOffset = m_sdlEvent.wheel.y;
			} break;
		}
	}
}

const tgState tgInput::getKeyboardState(int key) {
	auto pos = m_keyboard.find(key);
	if(pos != m_keyboard.end()) {
		return m_keyboard[key];
	}
	return{ false, false, false };
}

const tgState tgInput::getMouseState(int button) {
	auto pos = m_mouse.find(button);
	if(pos != m_mouse.end()) {
		return m_mouse[button];
	}
	return{ false, false, false };
}

bool tgInput::isKeyPressed(int key) {
	return getKeyboardState(key).pressed;
}

bool tgInput::isKeyReleased(int key) {
	return getKeyboardState(key).released;
}

bool tgInput::isKeyDown(int key) {
	return getKeyboardState(key).down;
}

bool tgInput::isMouseButtonPressed(int btn) {
	return getMouseState(btn).pressed;
}

bool tgInput::isMouseButtonReleased(int btn) {
	return getMouseState(btn).released;
}

bool tgInput::isMouseButtonDown(int btn) {
	return getMouseState(btn).down;
}
