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

#include <vector>
#include <cstdlib>

class Test : public tgGameState {
public:
	void start() override {
		getECS()->registerSystem(new tgSpriteSystem());
		getECS()->registerSystem(new tgTextSystem());

		tgEntity* e = getECS()->create();
		e->add<tgSprite>(tgAssets::getTexture("ball.png"));
		e->add<tgTransform>()->setLocalPosition(tgVector3(100, 100, 0));

		tgEntity* t = getECS()->create();
		t->add<tgText>("Hello World 1234567890", tgAssets::getFont("font.fnt"));
		t->add<tgTransform>()->setLocalPosition(tgVector3(10, 40, 0));
	}
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
