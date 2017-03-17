#define TG_DISABLE_ERROR_CHECK

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>

#include "core/tgWindow.h"
#include "core/tgInput.h"
#include "core/tgUtil.h"

#include "graphics/tgSpriteRenderer.h"

#include "core/tgImageData.h"
#include "graphics/tgTexture.h"

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow ("Test", 1024, 600);
	tgInput input;

	float timeDelta = 1.0f / 300.0f;
	float timeAccum = 0.0f;
	float startTime = float (SDL_GetTicks()) / 1000.0f;

	// Drawing tests
	tgSpriteRenderer *ren = new tgSpriteRenderer (1024, 600);

	std::ifstream f ("C:\\Users\\diego\\Documentos\\Visual Studio 2015\\Projects\\TGE\\Debug\\apple.png", std::ios::binary);
	tgImageData img (f);

	tgTexture *tex = new tgTexture (img, tgTexture::tgTEXTURE_RGBA);

	int frames = 0;
	int sz = 128;
	float ft = 0.0f;
	while (!win->shouldClose()) {
		bool canRender = false;
		float currentTime = float (SDL_GetTicks()) / 1000.0f;
		float delta = currentTime - startTime;
		startTime = currentTime;
		timeAccum += delta;

		input.update();
		if (input.isCloseRequested()) {
			win->close();
		}

		if(input.isKeyPressed(SDLK_SPACE)) {
			sz-=4;
			if(sz <= 8) {
				sz = 8;
			}
			Log("SZ: " << sz);
		}
		while (timeAccum >= timeDelta) {
			timeAccum -= timeDelta;
			canRender = true;
			// TODO: Update
			ft += timeDelta;
			if(ft >= 1.0f) {
				ft = 0.0f;
				Log(frames);
				frames = 0;
			}

		}

		if (canRender) {
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT);

			tgVector2 mp(input.getMouseX(), input.getMouseY());

			ren->begin();
			for(int i = 16; i < 1024; i += sz) {
				for(int j = 16; j < 600; j += sz) {
					ren->draw(tex,
						tgVector4(0,0,1,1),
						tgVector4(i, j, sz, sz),
						tgVector2(0.5f),
						0.0f
					);
				}
			}
			ren->end();

			win->swapBuffers();
			frames++; 
		} else {
			SDL_Delay (1);
		}
	}

	delete tex;
	delete ren;
	delete win;

	return 0;
}
