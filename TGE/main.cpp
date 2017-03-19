#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>

#include "core/tgWindow.h"
#include "core/tgInput.h"
#include "core/tgUtil.h"
#include "core/tgLog.h"

#include "graphics/tgSpriteRenderer.h"

#include "core/tgImageData.h"
#include "graphics/tgTexture.h"

#include "scenegraph/tgSceneTree.h"
#include "nodes/tgSpriteNode.h"

#include <vector>

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow ("Test", 1024, 600);
	tgInput input;
	
	float timeDelta = 1.0f / 300.0f;
	float timeAccum = 0.0f;
	float startTime = float (SDL_GetTicks()) / 1000.0f;

	tgSpriteRenderer *ren = new tgSpriteRenderer (1024, 600);

	std::ifstream f2 ("C:\\Users\\diego\\Documentos\\Visual Studio 2015\\Projects\\TGE\\Release\\apple.png", std::ios::binary);
	tgImageData apple_img (f2);
	tgTexture *apple_tex = new tgTexture (apple_img, tgTexture::tgTEXTURE_RGBA);

	tgSceneTree *stree = new tgSceneTree();
	tgSpriteNode *apple = new tgSpriteNode(apple_tex);
	stree->addChild(apple);

	apple->getTransform().setLocalPosition(tgVector3(512, 300, 0));

	int frames = 0;
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

		while (timeAccum >= timeDelta) {
			timeAccum -= timeDelta;
			canRender = true;
			
			stree->update(timeDelta);

			ft += timeDelta;
			if(ft >= 1.0f) {
				ft = 0.0f;
				frames = 0;
			}

		}

		if (canRender) {
			glClearColor(0.1f, 0.25f, 0.5f, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT);

			ren->begin();
			stree->render(ren);
			ren->end();

			win->swapBuffers();
			frames++; 
		} else {
			SDL_Delay (1);
		}
	}

	delete stree;
	delete ren;
	delete win;

	return 0;
}
