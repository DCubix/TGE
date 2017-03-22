#ifndef TIMER_H
#define TIMER_H

#include <queue>
#include <functional>

typedef struct tgTimeEvent {
	float time;
	std::function<void(void)> onComplete;
} tgTimeEvent;

class tgTimer {
public:

	static void wait(float secs, std::function<void(void)> const& onComplete = nullptr);
	static void update(float dt);

private:
	static std::queue<tgTimeEvent*> m_timers;
	static tgTimeEvent* m_currentTimer;
};

#endif
