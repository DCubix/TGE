#ifndef GAME_H
#define GAME_H

class tgEngine;
class tgGame {
public:
	virtual void start(tgEngine*) = 0;
	virtual void exit() = 0;
	virtual void update(float) {}
};

#endif
