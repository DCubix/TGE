#ifndef TWEENS_H
#define TWEENS_H

#include <vector>
#include <string>
#include <cmath>

#define PI 3.141592654f

#include "tgTween.h"

#define TG_EASE_FUNC_PARAMS float t, float b, float c, float d
#define TG_EASE_FUNC_DEF(inout, name) \
	static float ease##inout##name(TG_EASE_FUNC_PARAMS)

class tgTweens {
public:
	static void addTween(tgTween *tween);
	static void addTween(float *value, float end, float duration,
		std::function<void(void)> const& onComplete = nullptr,
		tgEasingFunction const& easing = nullptr);

	static void update(float dt);
protected:
	static std::vector<tgTween*> m_tweens;
};

namespace tgEasing {
	// Quadratic
	TG_EASE_FUNC_DEF(In, Quad) {
		return c*(t /= d)*t + b;
	}
	TG_EASE_FUNC_DEF(Out, Quad) {
		return -c *(t /= d)*(t - 2) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Quad) {
		if ((t /= d / 2) < 1) return c / 2 * t*t + b;
		return -c / 2 * ((--t)*(t - 2) - 1) + b;
	}

	// Cubic
	TG_EASE_FUNC_DEF(In, Cubic) {
		return c*(t /= d)*t*t + b;
	}
	TG_EASE_FUNC_DEF(Out, Cubic) {
		return c*((t = t / d - 1)*t*t + 1) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Cubic) {
		if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
		return c / 2 * ((t -= 2)*t*t + 2) + b;
	}

	// Quartic
	TG_EASE_FUNC_DEF(In, Quart) {
		return c*(t /= d)*t*t*t + b;
	}
	TG_EASE_FUNC_DEF(Out, Quart) {
		return -c * ((t = t / d - 1)*t*t*t - 1) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Quart) {
		if ((t /= d / 2) < 1) return c / 2 * t*t*t*t + b;
		return -c / 2 * ((t -= 2)*t*t*t - 2) + b;
	}

	// Quintic
	TG_EASE_FUNC_DEF(In, Quint) {
		return c*(t /= d)*t*t*t*t + b;
	}
	TG_EASE_FUNC_DEF(Out, Quint) {
		return c*((t = t / d - 1)*t*t*t*t + 1) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Quint) {
		if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
		return c / 2 * ((t -= 2)*t*t*t*t + 2) + b;
	}

	// Sine
	TG_EASE_FUNC_DEF(In, Sine) {
		return -c * std::cos(t / d * (PI / 2.0f)) + c + b;
	}
	TG_EASE_FUNC_DEF(Out, Sine) {
		return c * std::sin(t / d * (PI / 2.0f)) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Sine) {
		return -c / 2.0f * (std::cos(PI*t / d) - 1.0f) + b;
	}

	// Exponential
	TG_EASE_FUNC_DEF(In, Exp) {
		return (t == 0.0f) ? b : c * std::pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
	}
	TG_EASE_FUNC_DEF(Out, Exp) {
		return (t == d) ? b + c : c * (-std::pow(2.0f, -10.0f * t / d) + 1) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Exp) {
		if (t == 0.0f) return b;
		if (t == d) return b + c;
		if ((t /= d / 2.0f) < 1.0f) return c / 2.0f * std::pow(2.0f, 10.0f * (t - 1.0f)) + b;
		return c / 2.0f * (-std::pow(2.0f, -10.0f * --t) + 2.0f) + b;
	}

	// Circular
	TG_EASE_FUNC_DEF(In, Circular) {
		return -c * (std::sqrt(1.0f - (t /= d)*t) - 1.0f) + b;
	}
	TG_EASE_FUNC_DEF(Out, Circular) {
		return c * std::sqrt(1.0f - (t = t / d - 1.0f)*t) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Circular) {
		if ((t /= d / 2.0f) < 1.0f) return -c / 2.0f * (std::sqrt(1 - t*t) - 1.0f) + b;
		return c / 2.0f * (std::sqrt(1 - (t -= 2.0f)*t) + 1.0f) + b;
	}

	// Elastic
	TG_EASE_FUNC_DEF(In, Elastic) {
		float s = 1.70158f; float p = 0; float a = c;
		if (t == 0) return b;  if ((t /= d) == 1.0f) return b + c;  if (!p) p = d*0.3f;
		if (a < std::abs(c)) { a = c; float s = p / 4.0f; } else float s = p / (2.0f * PI) * std::asin(c / a);
		return -(a*std::pow(2.0f, 10.0f * (t -= 1)) * std::sin((t*d - s)*(2.0f * PI) / p)) + b;
	}
	TG_EASE_FUNC_DEF(Out, Elastic) {
		float s = 1.70158f; float p = 0; float a = c;
		if (t == 0.0f) return b;  if ((t /= d) == 1.0f) return b + c;  if (!p) p = d*0.3f;
		if (a < std::abs(c)) { a = c; float s = p / 4.0f; } else float s = p / (2.0f * PI) * std::asin(c / a);
		return a*std::pow(2.0f, -10.0f * t) * std::sin((t*d - s)*(2.0f * PI) / p) + c + b;
	}
	TG_EASE_FUNC_DEF(InOut, Elastic) {
		float s = 1.70158f; float p = 0; float a = c;
		if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;  if (!p) p = d*(0.3f*1.5f);
		if (a < std::abs(c)) { a = c; float s = p / 4; } else float s = p / (2 * PI) * std::asin(c / a);
		if (t < 1) return -0.5f*(a*std::pow(2.0f, 10.0f * (t -= 1)) * std::sin((t*d - s)*(2.0f * PI) / p)) + b;
		return a*std::pow(2.0f, -10.0f * (t -= 1.0f)) * std::sin((t*d - s)*(2.0f * PI) / p)*0.5f + c + b;
	}

	// Back
	TG_EASE_FUNC_DEF(In, Back) {
		float s = 1.70158f;
		return c*(t /= d)*t*((s + 1.0f)*t - s) + b;
	}
	TG_EASE_FUNC_DEF(Out, Back) {
		float s = 1.70158f;
		return c*((t = t / d - 1.0f)*t*((s + 1.0f)*t + s) + 1.0f) + b;
	}
	TG_EASE_FUNC_DEF(InOut, Back) {
		float s = 1.70158f;
		if ((t /= d / 2) < 1.0f) return c / 2 * (t*t*(((s *= (1.525f)) + 1.0f)*t - s)) + b;
		return c / 2 * ((t -= 2.0f)*t*(((s *= (1.525f)) + 1.0f)*t + s) + 2.0f) + b;
	}

	// Bounce
	TG_EASE_FUNC_DEF(Out, Bounce) {
		if ((t /= d) < (1.0f / 2.75f)) {
			return c*(7.5625f*t*t) + b;
		} else if (t < (2.0f / 2.75f)) {
			return c*(7.5625f*(t -= (1.5f / 2.75f))*t + 0.75f) + b;
		} else if (t < (2.5f / 2.75f)) {
			return c*(7.5625f*(t -= (2.25f / 2.75f))*t + 0.9375f) + b;
		} else {
			return c*(7.5625f*(t -= (2.625f / 2.75f))*t + 0.984375f) + b;
		}
	}

	TG_EASE_FUNC_DEF(In, Bounce) {
		return c - easeOutBounce(d - t, 0.0f, c, d) + b;
	}

	TG_EASE_FUNC_DEF(InOut, Bounce) {
		if (t < d * 0.5f) return easeInBounce(t * 2.0f, 0.0f, c, d) * 0.5f + b;
		return easeOutBounce(t * 2.0f - d, 0.0f, c, d) * 0.5f + c*0.5f + b;
	}
}

#endif