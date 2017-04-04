#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <queue>

typedef struct tgTimeEvent {
	float time = -1;
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
