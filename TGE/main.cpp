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
		getECS()->registerSystem(new tgTextSystem());

		t = getECS()->create();
		t->add<tgText>("Hello World 1234567890", tgAssets::getFont("font.fnt"));
		t->add<tgTransform>()->setLocalPosition(tgVector3(10, 40, 0));

		pe = new tgParticleEngine();
	}

	void update(float dt) override {
		if (tgInput::isKeyDown(SDLK_a)) {
			tgParticleConfiguration conf;
			conf.additive = true;
			conf.angle = float(-M_PI / 2);
			conf.speed = 30.0f;
			conf.startScale = 4.0f;
			conf.endScale = 0.5f;
			conf.startColor = tgVector4(0.60f, 0.2f, 0.05f, 1.0f);
			conf.startColorVar = conf.startColor;
			conf.endColor = tgVector4(0.0f, 0.0f, 0.0f, 1.0f);
			conf.endColorVar = conf.endColor;
			
			pe->emit(tgAssets::getTexture("particle.png"), tgVector2(320, 240), conf, [](tgVector2 const& v) {
				const float r = 100.0f;
				float d = r * 2.0f;

				float x = std::cos(v.x()) * d;
				float y = std::sin(v.x()) * d;

				float s = std::sqrt((d * d) / 2.0f);

				if (x > s) { x = s; }
				if (x < -s) { x = -s; }
				if (y > s) { y = s; }
				if (y < -s) { y = -s; }
				return tgVector2(x, y);
			});
		}

		if (tgInput::isCloseRequested()) {
			getEngine()->stop();
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

	tgEntity *t;
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
