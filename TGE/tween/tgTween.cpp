#include "tgTween.h"

#include "../core/tgUtil.h"

tgTween::tgTween(float duration, tgEasingFunction const & easing, std::function<void(void)> const & complete) 
	: m_finished(false),
	m_duration(duration),
	m_easing(easing),
	m_onComplete(complete)
{
}

tgTween::tgTween(float duration, float *value, float target, tgEasingFunction const& easing, std::function<void(void)> const& complete)
	: m_finished(false),
	m_duration(duration),
	m_easing(easing),
	m_onComplete(complete)
{
	addValue(value, target);
}

tgTween::~tgTween() {
}

void tgTween::addValue(float *value, float target) {
	m_values.push_back(tgValue(value, target));
}

void tgTween::update(float dt) {
	float before = m_time;
	m_time += dt;
	for (tgValue &val : m_values) {
		if (before <= FLT_EPSILON) {
			if (val.value) {
				val.start = *val.value;
			}
		}

		if (val.value) {
			if (m_easing) {
				*val.value = m_easing(m_time, val.start, val.end - val.start, m_duration);
			} else { // Just uses a simple Lerp
				float t = m_time / m_duration;
				*val.value = (1.0f - t) * val.start + val.end * t;
			}
		}

		if (m_time >= m_duration) {
			if (val.value) {
				*val.value = val.end;
			}
			m_finished = true;
			if (m_onComplete) { m_onComplete(); }
		}
	}
}

tgValue::tgValue(float *value, float end) {
	this->value = value;
	this->end = end;
	this->start = 0;
}
