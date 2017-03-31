#include <string>
#include <fstream>
#include <sstream>

#include "core/tgEngine.h"
#include "core/tgLog.h"
#include "core/tgInput.h"

#include "math/tgMath.h"

#include "ecs/tgComponent.h"
#include "ecs/tgMessenger.h"

#include "components/tgSpriteComponent.h"
#include "components/tgFontComponent.h"
#include "components/tgTransformComponent.h"
#include "components/tgEmitterComponent.h"
#include "components/tgAudioSourceComponent.h"

#include "assets/tgAssets.h"

#include "tween/tgTweens.h"
#include "tween/tgTimer.h"

#include <vector>
#include <cstdlib>

static float score = 0;

float randF(float LO, float HI) {
	return LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
}

tgVector4 hsv(float h, float s, float v) {
	float r, g, b;

	int i = int(std::floor(h * 6));
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

class Block : public tgComponent {
public:
	Block() {
		killing = false;
		animation_speed = 2.0f;
		time = 0.0f;
	}

	void receive(tgMessage const& msg) {
		if (msg.text == "collided_with_ball") {
			tgTransform *block_t = getManager()->getComponent<tgTransformComponent>(getOwner())->getTransform();

			tgAudioSourceComponent *ascomp = getManager()->getComponent<tgAudioSourceComponent>(getOwner());
			tgAudioSource *k = ascomp->play("bounce");
			k->setPitch(randF(0.95f, 1.1f));
			k->setVolume(1.3f);

			tgTweens::addTween(&block_t->getLocalScaling().x(), 0.0f, 0.4f, [&]() {
				getManager()->destroyEntity(getOwner());
			}, tgEasing::easeInBack);
		}
	}

	float animation_speed;
	float time;
	bool killing;
};

class Paddle : public tgComponent {
public:
	Paddle() {
		speed = 0.0f;
		maxSpeed = 280.0f;
	}

	void update(float dt) {
		speed = 0;
		if (tgInput::isKeyDown(SDLK_LEFT)) {
			speed = -maxSpeed;
		} else if (tgInput::isKeyDown(SDLK_RIGHT)) {
			speed = maxSpeed;
		}

		auto tc = getManager()->getComponent<tgTransformComponent>(getOwner());
		tgTransform *t = tc->getTransform();
		t->setLocalPosition(t->getLocalPosition() + tgVector3(speed, 0, 0) * dt);
	}

	void receive(tgMessage const& msg) {
		if (msg.text == "collided_with_ball") {
			auto tc = getManager()->getComponent<tgTransformComponent>(getOwner());
			tgTransform *t = tc->getTransform();

			tgAudioSourceComponent *ascomp = getManager()->getComponent<tgAudioSourceComponent>(getOwner());
			tgAudioSource *k = ascomp->play("knock");
			k->setPitch(randF(0.95f, 1.1f));
			k->setVolume(1.3f);

			tgTweens::addTween(&tc->getTransform()->getLocalPosition().y(), 430.0f, 0.2f, nullptr, tgEasing::easeOutElastic);
			tgTimer::wait(0.06f, [tc]() {
				tgTweens::addTween(&tc->getTransform()->getLocalPosition().y(), 425.0f, 0.8f, nullptr, tgEasing::easeOutElastic);
			});
		}
	}

	float speed, maxSpeed;
};

class Ball : public tgComponent {
public:
	Ball() {
		direction = tgVector2(-1.0f, -1.0f);
		speed = 250.0f;
	}

	void update(float dt) override {
		auto t_comp = getManager()->getComponent<tgTransformComponent>(getOwner());
		tgTransform *t = t_comp->getTransform();

		tgVector3 pos = t->getLocalPosition();
		if (pos.y() < 9) {
			direction.y() *= -1;
		} else if (pos.y() > 488) {
			pos.x() = 320;
			pos.y() = 400;
			t->setLocalPosition(pos);
			direction = tgVector2(randF(-1.0f, 1.0f), -1);
		}

		if (pos.x() < 9) {
			direction.x() *= -1;
		} else if (pos.x() > 631) {
			direction.x() *= -1;
		}

		t->setLocalPosition(tgVector3(t->getLocalPosition().xy() + direction * speed * dt, 0));

		for (tgEntity ent : getManager()->search<tgTransformComponent, Paddle>()) {
			Paddle *paddle = getManager()->getComponent<Paddle>(ent);
			auto paddle_tc = getManager()->getComponent<tgTransformComponent>(ent);

			tgVector3 bpos = t->getLocalPosition();
			tgVector3 ppos = paddle_tc->getTransform()->getLocalPosition();

			float w = 0.5 * (16 + 96);
			float h = 0.5 * (16 + 16);
			float dx = bpos.x() - ppos.x();
			float dy = bpos.y() - (ppos.y() + 8);

			if (std::abs(dx) <= w && std::abs(dy) <= h) {
				tgVector2 N(0, 1);

				/* collision! */
				float wy = w * dy;
				float hx = h * dx;

				if (wy > hx) {
					if (wy > -hx) {
						N.x() = 0; N.y() = -1;
					} else {
						N.x() = -1; N.y() = 0;
					}
				} else {
					if (wy > -hx) {
						N.x() = 1; N.y() = 0;
					} else {
						N.x() = 0; N.y() = 1;
					}
				}

				direction = direction.reflect(N).normalized();

				tgMessenger *msgr = getManager()->getMessenger();
				msgr->sendMessage(getOwner(), ent, "collided_with_ball");
			}
		}

		for (tgEntity ent : getManager()->search<tgTransformComponent, Block>()) {
			Block *block = getManager()->getComponent<Block>(ent);
			auto block_tc = getManager()->getComponent<tgTransformComponent>(ent);

			if (block->killing) { continue; }

			tgVector3 bpos = t->getLocalPosition();
			tgVector3 blpos = block_tc->getTransform()->getLocalPosition();

			float w = 0.5 * (16 + 48);
			float h = 0.5 * (16 + 16);
			float dx = bpos.x() - blpos.x();
			float dy = bpos.y() - blpos.y();

			if (std::abs(dx) <= w && std::abs(dy) <= h && !block->killing) {
				block->killing = true;

				tgVector2 N(0, 1);

				/* collision! */
				float wy = w * dy;
				float hx = h * dx;

				if (wy > hx) {
					if (wy > -hx) {
						N.x() = 0; N.y() = -1;
					} else {
						N.x() = -1; N.y() = 0;
					}
				} else {
					if (wy > -hx) {
						N.x() = 1; N.y() = 0;
					} else {
						N.x() = 0; N.y() = 1;
					}
				}

				direction = direction.reflect(N).normalized();
				score += 100;

				tgMessenger *msgr = getManager()->getMessenger();
				msgr->sendMessage(getOwner(), ent, "collided_with_ball");

				break;
			}
		}
	}

	tgVector2 direction;
	float speed;
};

class Breakout : public tgGameState {
public:
	Breakout() : tgGameState() {
		audioSystem = new tgAudioSystem();
	}

	void start(tgEngine* engine) override {
		this->engine = engine;
		tgWindow *win = engine->getWindow();
		tgComponentManager *mgr = getManager();

		tgEntity ball = mgr->createEntity();
		mgr->addComponent<Ball>(ball);
		tgTransform *ball_t = mgr->addComponent<tgTransformComponent>(ball)->getTransform();
		ball_t->setLocalPosition(tgVector3(320, 400, 2));
		mgr->addComponent<tgSpriteComponent>(ball, tgAssets::getTexture("ball.png"), tgVector2(0.5f));

		//// Create a simple particle emitter
		tgEmitterComponent *pemit = new tgEmitterComponent(
			tgAssets::getTexture("particle.png"), 100
		);

		tgEmitterConfiguration conf;
		conf.startColor = tgVector4(0.8f, 0.45f, 0.2f, 1.0f);
		conf.startColorVar = conf.startColor;
		conf.endColor = tgVector4(0.0f);
		conf.endColorVar = conf.endColor;
		conf.emissionRate = 50;
		conf.life = 1.2f;
		conf.startScale = 2.0f;

		pemit->setAdditive(true);
		pemit->setConfiguration(conf);
		pemit->setRenderingOrder(-0.1f);

		mgr->addComponent(ball, pemit);
		////

		tgEntity paddle = mgr->createEntity();
		
		tgAudioSourceComponent* ascomp = mgr->addComponent<tgAudioSourceComponent>(paddle, audioSystem);
		ascomp->add("knock", tgAssets::getSound("knock.ogg"));

		mgr->addComponent<Paddle>(paddle);
		tgTransform *paddle_t = mgr->addComponent<tgTransformComponent>(paddle)->getTransform();
		paddle_t->setLocalPosition(tgVector3(320, 425, 1));
		mgr->addComponent<tgSpriteComponent>(paddle, tgAssets::getTexture("paddle.png"), tgVector2(0.5f, 0.0f));

		tgEntity scoreText = mgr->createEntity();
		tgTransform *score_t = mgr->addComponent<tgTransformComponent>(scoreText)->getTransform();
		score_t->setLocalPosition(tgVector3(20, 10, 99));
		score_t->setLocalScaling(tgVector3(0.55f, 0.55f, 1.0f));
		score_font = mgr->addComponent<tgFontComponent>(scoreText, "Score: 0", tgAssets::getFont("font.fnt"));

		// Create blocks
		const int spacing = 4;
		const int padding = 16;

		int blockWidth = 48 + spacing;
		int blockHeight = 16 + spacing;
		int nblockX = (win->getWidth() - padding * 2) / blockWidth;
		int nblockY = (win->getHeight() / 3 - padding) / blockHeight;

		tgTexture* block_tex = tgAssets::getTexture("block.png");
		float h = 0;
		float step = 1.0f / float(nblockX);

		for (int i = 0; i < nblockX; i++) {
			for (int j = 0; j < nblockY; j++) {
				tgEntity block = mgr->createEntity();
				mgr->addComponent<Block>(block);

				tgTransform *block_t = mgr->addComponent<tgTransformComponent>(block)->getTransform();
				block_t->setLocalPosition(tgVector3(padding * 2 + 24 + i * blockWidth, padding * 2 + 8 + j * blockHeight, 0));
				block_t->setLocalScaling(tgVector3(0, 0, 1));
				tgSpriteComponent *block_s = mgr->addComponent<tgSpriteComponent>(block, block_tex, tgVector2(0.5f));
				tgAudioSourceComponent *ascomp = mgr->addComponent<tgAudioSourceComponent>(block, audioSystem);
				ascomp->add("bounce", tgAssets::getSound("bounce.ogg"));

				block_s->setColor(hsv(h, 0.5f, 1.0f));
				h += step;
				if (h >= 1.0f) {
					h = 0.0f;
				}

				tgTween *t = new tgTween(1.0f, tgEasing::easeOutElastic);
				t->addValue(&block_t->getLocalScaling().x(), 1.0f);
				t->addValue(&block_t->getLocalScaling().y(), 1.0f);
				tgTweens::addTween(t);
			}
		}
		/////////
	}

	void update(float dt) override {
		std::stringstream stm;
		stm << "Score: " << score;
		score_font->setText(stm.str());

		if (tgInput::isCloseRequested()) {
			engine->stop();
		}

		audioSystem->update();
	}

	tgFontComponent *score_font;
	tgAudioSystem *audioSystem;
	tgEngine* engine;
};

class Menu : public tgGameState {
public:
	void start(tgEngine* engine) override {
		this->engine = engine;
		tgWindow *win = engine->getWindow();
		tgComponentManager *mgr = getManager();

		tgEntity mtext = mgr->createEntity();
		tgTransform *t = mgr->addComponent<tgTransformComponent>(mtext)->getTransform();
		t->setLocalPosition(tgVector3(66, 232, 99));
		mgr->addComponent<tgFontComponent>(mtext, "Press [ENTER] to Start", tgAssets::getFont("font.fnt"));
	}

	void update(float dt) override {
		if (tgInput::isKeyPressed(SDLK_RETURN)) {
			engine->setState("play");
		}
		if (tgInput::isCloseRequested()) {
			engine->stop();
		}
	}
	tgEngine* engine;
};

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow("BreakOUT!", 640, 480);
	tgEngine *engine = new tgEngine(win);

	tgSpriteBatch *sb = new tgSpriteBatch(640, 480);
	engine->setRenderer(sb);
/*
	engine->setAssetPreloadFunction([&]() {
		tgAssets::add<tgTextureAsset>("ball.png");
		tgAssets::add<tgTextureAsset>("paddle.png");
		tgAssets::add<tgTextureAsset>("block.png");
		tgAssets::add<tgTextureAsset>("particle.png");
		tgAssets::add<tgFontAsset>("font.fnt");
		tgAssets::add<tgAudioAsset>("knock.ogg");
		tgAssets::add<tgAudioAsset>("bounce.ogg");

		std::string src = *static_cast<std::string*>(tgAssets::loadSingle<tgTextAsset>("bloom.glsl"));
		tgPostEffect *fx = new tgPostEffect();
		fx->setShader(src);

		sb->addPostEffect(fx);
	});
*/
	engine->registerState("menu", new Menu());
	engine->registerState("play", new Breakout());

	engine->start();

	return 0;
}
