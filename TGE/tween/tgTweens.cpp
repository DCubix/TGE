#include "tgTweens.h"

#include "../core/tgUtil.h"
#include "../core/tgLog.h"

#include <algorithm>

std::vector<tgTween*> tgTweens::m_tweens;

void tgTweens::addTween(tgTween *tween) {
	m_tweens.push_back(tween);
}

void tgTweens::addTween(float *value, float end, float duration, std::function<void(void)> const& onComplete, tgEasingFunction const& easing) {
	addTween(new tgTween(duration, value, end, easing, onComplete));
}

void tgTweens::update(float dt) {
	std::vector<tgTween*> rem;
	for (tgTween *t : m_tweens) {
		t->update(dt);

		if (t->m_finished) {
			rem.push_back(t);
		}
	}
	for (tgTween *t : rem) {
		m_tweens.erase(std::remove(m_tweens.begin(), m_tweens.end(), t), m_tweens.end());
		tgDelete(t);
	}
}
