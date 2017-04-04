#include <string>
#include <fstream>
#include <sstream>

#include "core/tgEngine.h"
#include "core/tgLog.h"
#include "core/tgInput.h"

#include "graphics/tgSpriteBatch.h"

#include "math/tgMath.h"

#include "assets/tgAssets.h"

#include "tween/tgTweens.h"
#include "tween/tgTimer.h"

#include "ecs/tgECS.h"
#include "systems/tgSpriteSystem.h"
#include "systems/tgTextSystem.h"
#include "components/tgSprite.h"
#include "components/tgText.h"
#include "components/tgTransform.h"

#include "core/tgParticleEngine.h"

#include <vector>
#include <cstdlib>

class Test : public tgGameState {
public:
	void start() override {
		pe = new tgParticleEngine();
	}

	void update(float dt) override {
		tgParticleConfiguration conf;
		conf.additive = true;
		conf.angle = float(-M_PI / 2);
		conf.speed = 300.0f;
		conf.life = 0.5f;
		conf.startScale = 1.0f;
		conf.startScaleVar = 1.0f;
		conf.endScale = 0.0f;
		conf.endScaleVar = 0.5f;
		conf.positionVar = tgVector2(10.0f);
		conf.startColor = tgVector4(0.05f, 0.2f, 0.5f, 1.0f);
		conf.startColorVar = conf.startColor;
		conf.endColor = tgVector4(0.5f, 0.2f, 0.05f, 1.0f);
		conf.endColorVar = conf.endColor;
		conf.angularSpeed = 10.0f;
		conf.angularSpeedVar = -20.0f;

		pe->emit(tgAssets::getTexture("fire.png"), tgVector2(tgInput::getMouseX(), tgInput::getMouseY()), conf);

		if (tgInput::isMouseButtonDown(SDL_BUTTON_LEFT)) {
			getEngine()->setTimeScale(0.2f);
		} else {
			getEngine()->setTimeScale(1.0f);
		}

		pe->update(dt);
	}

	void render(tgRenderer *r) override {
		pe->render(dynamic_cast<tgSpriteBatch*>(r));
	}

	~Test() {
		tgGameState::~tgGameState();
		tgDelete(pe);
	}

	tgParticleEngine *pe;
};

int main (int argc, char **argv) {
	tgWindow *win = new tgWindow("TGE", 640, 480);
	tgEngine *engine = new tgEngine(win);

	tgSpriteBatch *sb = new tgSpriteBatch(640, 480);
	engine->setRenderer(sb);
	engine->registerState("main", new Test());

	engine->start();

	return 0;
}
