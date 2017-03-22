#include "tgTime.h"

float tgTime::getTime() {
	return float(SDL_GetTicks()) / 1000.0f;
}

void tgTime::delay(float secs) {
	SDL_Delay(int(secs * 1000.0f));
}
