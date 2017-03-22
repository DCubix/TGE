#ifndef TWEEN_H
#define TWEEN_H

#include <vector>
#include <functional>

using tgEasingFunction = std::function<float(float, float, float, float)>;

class tgTweens;

typedef struct tgValue {
	tgValue(float *value, float end);

	float *value;
	float end, start;
} tgValue;

class tgTween {
	friend class tgTweens;
public:
	tgTween(float duration,
		tgEasingFunction const& easing = nullptr,
		std::function<void(void)> const& complete = nullptr);
	tgTween(float duration, float *value, float target,
		tgEasingFunction const& easing = nullptr,
		std::function<void(void)> const& complete = nullptr);
	~tgTween();

	void addValue(float *value, float target);

	void update(float dt);

protected:
	bool m_finished;

	std::function<void(void)> m_onComplete;
	tgEasingFunction m_easing;

	std::vector<tgValue*> m_values;

	float m_duration; ///< Duration in seconds
	float m_time;
};

#endif