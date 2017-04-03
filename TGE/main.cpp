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
		getECS()->registerSystem(new tgSpriteSystem());
		getECS()->registerSystem(new tgTextSystem());

		e = getECS()->create();
		e->add<tgSprite>(tgAssets::getTexture("ball.png"));
		e->add<tgTransform>()->setLocalPosition(tgVector3(100, 100, 0));

		t = getECS()->create();
		t->add<tgText>("Hello World 1234567890", tgAssets::getFont("font.fnt"));
		auto transform = t->add<tgTransform>();
		transform->setLocalPosition(tgVector3(10, 40, 0));
		transform->setLocalScaling(tgVector3(0, 0, 1));

		tgTween *t = new tgTween(2.0f, tgEasing::easeInOutBack);
		t->addValue(&transform->getLocalScaling().x(), 1.0f);
		t->addValue(&transform->getLocalScaling().y(), 1.0f);
		tgTweens::addTween(t);

		pe = new tgParticleEngine();
	}

	void update(float dt) override {
		if (tgInput::isKeyDown(SDLK_a)) {
			tgParticleConfiguration conf;
			conf.angleVar = float(M_PI * 2);
			conf.speed = 50.0f;
			conf.startColor = tgVector4(1.0f, 0.6f, 0.0f, 1.0f);
			conf.endColor = tgVector4(0.0f, 0.6f, 1.0f, 1.0f);
			pe->emit(tgAssets::getTexture("ball.png"), tgVector2(200, 200), conf);
		}
		if (tgInput::isKeyPressed(SDLK_SPACE)) {
			e->destroy();
			t->get<tgText>()->setText("Deleted the Ball");
		}

		if (tgInput::isCloseRequested()) {
			getEngine()->stop();
		}

		pe->update(dt);
	}

	void render(tgRenderer *r) override {
		pe->render(dynamic_cast<tgSpriteBatch*>(r));
	}

	tgEntity *e, *t;
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
