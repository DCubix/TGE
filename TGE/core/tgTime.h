#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>

class tgTime {
public:
	static float getTime();
	static void delay(float secs);
};

#endif