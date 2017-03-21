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

#include <vector>

class Position : public tgComponent {
public:
	Position() : position(tgVector2(0.0f)) {}
	Position(tgVector2 pos) : position(pos) {}

	tgVector2 position;
};

class tgTestSystem : public tgSystem {
public:
	void update(tgWorld *world, float dt) {
		world->forEach<Position>([&](tgEntity *e, tgHandler<Position> pos) {
			tgLog::log(pos->position, " from entity #", e->getID());
		});
	}
};

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow ("Test", 320, 240);
	tgInput input;
	
	tgWorld *ecs_world = new tgWorld();
	ecs_world->addSystem(new tgTestSystem());

	tgEntity *test = ecs_world->create();
	test->add<Position>();

	float timeDelta = 1.0f / 300.0f;
	float timeAccum = 0.0f;
	float startTime = float (SDL_GetTicks()) / 1000.0f;

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
			
			ecs_world->update(timeDelta);

			ft += timeDelta;
			if(ft >= 1.0f) {
				ft = 0.0f;
				frames = 0;
			}
		}

		if (canRender) {
			glClearColor(0.1f, 0.25f, 0.5f, 1.0f);
			glClear (GL_COLOR_BUFFER_BIT);

			win->swapBuffers();
			frames++; 
		} else {
			SDL_Delay (1);
		}
	}

	delete ecs_world;
	delete win;

	return 0;
}
