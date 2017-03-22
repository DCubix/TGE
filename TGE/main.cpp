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

#include "tween/tgTweens.h"
#include "tween/tgTimer.h"

#include <vector>
#include <cstdlib>

float randF(float LO, float HI) {
	return LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
}

tgVector4 hsv(float h, float s, float v) {
	float r, g, b;

	int i = std::floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}

	return tgVector4(r, g, b, 1.0f);
}

class Ball : public tgComponent {
public:
	Ball() {
		direction = tgVector2(0.0f, -1.0f);
		speed = 200.0f;
	}
	tgVector2 direction;
	float speed;
};

class Block : public tgComponent {
public:
	Block() {
		killing = false;
		animation_speed = 2.0f;
		time = 0.0f;
	}

	float animation_speed;
	float time;
	bool killing;
};

class Paddle : public tgComponent {
public:
	Paddle() {
		frictionCoefficient = 0.98f;
		speed = 0.0f;
		maxSpeed = 260.0f;
	}

	float frictionCoefficient;
	float speed, maxSpeed;
};

class BallMovement : public tgSystem {
public:
	void update(tgEntitySystemManager *m, float dt) {
		for (tgEntity *ent : m->search<Ball, tgTransformComponent>()) {
			tgHandler<Ball> ball = ent->get<Ball>();
			tgHandler<tgTransformComponent> tc = ent->get<tgTransformComponent>();

			tgTransform *t = tc->getTransform();

			tgVector3 pos = t->getLocalPosition();
			if (pos.y() < 9) {
				ball->direction.y() *= -0.98f;
			} else if (pos.y() > 488) {
				pos.x() = 320;
				pos.y() = 400;
				t->setLocalPosition(pos);
				ball->direction = tgVector2(0, -1);
			}

			if (pos.x() < 9) {
				ball->direction.x() *= -0.98f;
			} else if (pos.x() > 631) {
				ball->direction.x() *= -0.98f;
			}

			t->setLocalPosition(tgVector3(t->getLocalPosition().xy() + ball->direction * ball->speed * dt, 0));
		}

		for (tgEntity *ball_e : m->search<Ball, tgTransformComponent>()) {
			tgHandler<Ball> ball = ball_e->get<Ball>();
			tgHandler<tgTransformComponent> ball_tc = ball_e->get<tgTransformComponent>();

			for (tgEntity *pad_e : m->search<Paddle, tgTransformComponent>()) {
				tgHandler<Paddle> paddle = pad_e->get<Paddle>();
				tgHandler<tgTransformComponent> paddle_tc = pad_e->get<tgTransformComponent>();

				tgVector3 bpos = ball_tc->getTransform()->getLocalPosition();
				tgVector3 ppos = paddle_tc->getTransform()->getLocalPosition();

				if (bpos.x() + 8 >= ppos.x() - 48 &&
					bpos.x() - 8 <= ppos.x() + 48 &&
					bpos.y() + 8 >= ppos.y() &&
					bpos.y() - 8 <= ppos.y() + 16)
				{
					float pad_dir = paddle->speed > 0 ? 1 : -1;
					if (paddle->speed == 0) {
						pad_dir = 0;
					}
					float facing = bpos.y() > ppos.y() ? 1 : -1;
					ball->direction = ball->direction.reflect(tgVector2(0, facing)).normalized();
					ball->direction.x() += pad_dir * paddle->frictionCoefficient;
				}
			}
		}

		for (tgEntity *ball_e : m->search<Ball, tgTransformComponent>()) {
			tgHandler<Ball> ball = ball_e->get<Ball>();
			tgHandler<tgTransformComponent> ball_tc = ball_e->get<tgTransformComponent>();

			for (tgEntity *block_e : m->search<Block, tgTransformComponent>()) {
				tgHandler<Block> block = block_e->get<Block>();
				tgHandler<tgTransformComponent> block_tc = block_e->get<tgTransformComponent>();
				
				if (block->killing) { continue; }

				tgVector3 bpos = ball_tc->getTransform()->getLocalPosition();
				tgVector3 blpos = block_tc->getTransform()->getLocalPosition();

				if (bpos.x() + 8 >= blpos.x() - 24 &&
					bpos.x() - 8 <= blpos.x() + 24 &&
					bpos.y() + 8 >= blpos.y() - 8 &&
					bpos.y() - 8 <= blpos.y() + 8)
				{
					tgVector2 N(0, 1);
					if (bpos.x() >= blpos.x() && bpos.x() <= blpos.x() + 48 && bpos.y() > blpos.y()) {
						N.y() = 1;
					} else if (bpos.x() >= blpos.x() && bpos.x() <= blpos.x() + 48 && bpos.y() < blpos.y()) {
						N.y() = -1;
					} else if (bpos.y() >= blpos.y() && bpos.y() <= blpos.y() - 16 && bpos.x() > blpos.x()) {
						N.x() = 1;
					} else {
						N.x() = -1;
					}
					ball->direction = ball->direction.reflect(N).normalized() * 0.97f;
					
					tgTweens::addTween(&block_tc->getTransform()->getLocalScaling().x(), 0.0f, 0.4f, [block_e]() {
						block_e->kill();
					}, tgEasing::easeInBack);

					break;
				}
			}
		}
	}
};

class PaddleMovement : public tgSystem {
public:
	void update(tgEntitySystemManager *m, float dt) {
		for (tgEntity *ent : m->search<Paddle, tgTransformComponent>()) {
			tgHandler<Paddle> paddle = ent->get<Paddle>();
			tgHandler<tgTransformComponent> tc = ent->get<tgTransformComponent>();

			tgTransform *t = tc->getTransform();
			t->setLocalPosition(t->getLocalPosition() + tgVector3(paddle->speed, 0, 0) * dt);
		}
	}
};

class InputSystem : public tgSystem {
public:
	InputSystem() {}

	void fixedUpdate(tgEntitySystemManager *m, float dt) {
		for (tgEntity *ent : m->search<Paddle>()) {
			tgHandler<Paddle> paddle = ent->get<Paddle>();
			
			paddle->speed = 0;
			if (tgInput::isKeyDown(SDLK_LEFT)) {
				paddle->speed = -paddle->maxSpeed;
			} else if (tgInput::isKeyDown(SDLK_RIGHT)) {
				paddle->speed = paddle->maxSpeed;
			}
		}
	}

};

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow ("Test", 640, 480);

	tgAssets::create();
	tgAssets::addSource(".");

	tgAssets::add<tgTextureAsset>("ball.png");
	tgAssets::add<tgTextureAsset>("paddle.png");
	tgAssets::add<tgTextureAsset>("block.png");

	tgAssets::load();

	///////// Using the ECS
	tgEntitySystemManager *ecs_world = new tgEntitySystemManager();
	tgSpriteRenderingSystem *ren = new tgSpriteRenderingSystem(new tgSpriteBatch(640, 480));
	ecs_world->addSystem(ren);
	ecs_world->addSystem(new InputSystem());
	ecs_world->addSystem(new PaddleMovement());
	ecs_world->addSystem(new BallMovement());

	tgEntity *ball = ecs_world->create();
	ball->add<Ball>();
	tgHandler<tgTransformComponent> ball_t = ball->add<tgTransformComponent>();
	ball_t->getTransform()->setLocalPosition(tgVector3(320, 400, 0));
	ball->add<tgSpriteComponent>(tgAssets::get<tgTexture>("ball.png"), tgVector2(0.5f));
	
	tgEntity *paddle = ecs_world->create();
	paddle->add<Paddle>();
	tgHandler<tgTransformComponent> paddle_t = paddle->add<tgTransformComponent>();
	paddle_t->getTransform()->setLocalPosition(tgVector3(320, 425, 0));
	paddle->add<tgSpriteComponent>(tgAssets::get<tgTexture>("paddle.png"), tgVector2(0.5f, 0.0f));

	// Create blocks
	const int spacing = 4;
	const int padding = 16;

	int blockWidth = 48 + spacing;
	int blockHeight = 16 + spacing;
	int nblockX = (win->getWidth() - padding * 2) / blockWidth;
	int nblockY = (win->getHeight()/3 - padding) / blockHeight;

	tgTexture *block_tex = tgAssets::get<tgTexture>("block.png");
	float h = 0;
	float step = 1.0f / float(nblockX);

	for (int i = 0; i < nblockX; i++) {
		tgTimer::wait(0.02f, [&, i, ecs_world, nblockY]() {
			for (int j = 0; j < nblockY; j++) {
				tgEntity *block = ecs_world->create();
				block->add<Block>();
				tgHandler<tgTransformComponent> block_t = block->add<tgTransformComponent>();
				block_t->getTransform()->setLocalPosition(tgVector3(padding * 2 + 24 + i * blockWidth, padding * 2 + 8 + j * blockHeight, 0));
				block_t->getTransform()->setLocalScaling(tgVector3(0, 0, 1));
				tgHandler<tgSpriteComponent> block_s = block->add<tgSpriteComponent>(block_tex, tgVector2(0.5f));

				block_s->setColor(hsv(h, 0.5f, 1.0f));
				h += step;
				if (h >= 1.0f) {
					h = 0.0f;
				}

				tgTween *t = new tgTween(1.0f, tgEasing::easeOutElastic);
				t->addValue(&block_t->getTransform()->getLocalScaling().x(), 1.0f);
				t->addValue(&block_t->getTransform()->getLocalScaling().y(), 1.0f);
				tgTweens::addTween(t);

			}
		});
	}
	
	/////////

	float timeDelta = 1.0f / 60.0f;
	float timeAccum = 0.0f;
	float startTime = float (SDL_GetTicks()) / 1000.0f;

	int frames = 0;
	float ft = 0.0f;

	bool canRender = false;
	while (!win->shouldClose()) {
		canRender = false;
		float currentTime = float (SDL_GetTicks()) / 1000.0f;
		float delta = currentTime - startTime;
		startTime = currentTime;
		timeAccum += delta;

		tgInput::update();
		if (tgInput::isCloseRequested()) {
			win->close();
		}

		ecs_world->fixedUpdate(timeDelta);
		
		while (timeAccum >= timeDelta) {
			timeAccum -= timeDelta;
			canRender = true;
			
			ecs_world->update(timeDelta);

			tgTimer::update(timeDelta);
			tgTweens::update(timeDelta);
			
			ft += timeDelta;
			if(ft >= 1.0f) {
				ft = 0.0f;
				tgLog::println(frames);
				frames = 0;
			}
		}

		if (canRender) {
			glClearColor(0.1f, 0.25f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			ecs_world->render();

			win->swapBuffers();
			frames++;
		} else {
			SDL_Delay(1);
		}
	}

	tgAssets::destroy();

	delete ecs_world;
	delete win;

	return 0;
}
