#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>

class tgTime {
public:
	float getTime();
	void delay(float secs);
};

#endif