#include <string>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>

#include "core/tgWindow.h"
#include "core/tgInput.h"
#include "core/tgUtil.h"
#include "core/tgLog.h"
#include "math/tgMath.h"

#include "graphics/tgGL.h"

#include "ecs/tgECS.h"
#include "systems/tgSpriteRenderingSystem.h"
#include "components/tgSpriteComponent.h"
#include "components/tgTransformComponent.h"

#include "assets/tgAssets.h"
#include "assets/tgTextureAsset.h"

#include <vector>

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow ("Test", 320, 240);
	tgInput input;
	
	tgAssets::create();
	tgAssets::addSource(".");

	tgAssets::add<tgTextureAsset>("apple.png");

	tgAssets::load();

	///////// Using the ECS
	tgEntitySystemManager *ecs_world = new tgEntitySystemManager();
	tgSpriteRenderingSystem *ren = new tgSpriteRenderingSystem(320, 240);
	ecs_world->addSystem(ren);

	tgEntity *apple = ecs_world->create();
	tgHandler<tgTransformComponent> apple_transform = apple->add<tgTransformComponent>();
	apple_transform->getTransform()->setLocalPosition(tgVector3(160, 120, 0));

	tgTexture *tex = tgAssets::get<tgTexture>("apple.png");

	tgHandler<tgSpriteComponent> apple_sprite = apple->add<tgSpriteComponent>(tex, tgVector2(0.5f));
	/////////

	float timeDelta = 1.0f / 300.0f;
	float timeAccum = 0.0f;
	float startTime = float (SDL_GetTicks()) / 1000.0f;

	int frames = 0;
	float ft = 0.0f;

	while (!win->shouldClose()) {
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
			
			ecs_world->update(timeDelta);
			apple_transform->getTransform()->rotate(tgVector3(0, 0, 1), timeDelta * 2);

			ft += timeDelta;
			if(ft >= 1.0f) {
				ft = 0.0f;
				tgLog::println(frames);
				frames = 0;
			}
		}

		glClearColor(0.1f, 0.25f, 0.5f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);

		ecs_world->render();

		win->swapBuffers();
		frames++;
	}

	tgAssets::destroy();

	delete ecs_world;
	delete win;

	return 0;
}
