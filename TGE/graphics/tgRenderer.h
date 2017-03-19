#ifndef RENDERER_H
#define RENDERER_H

class tgRenderer {
public:
	virtual ~tgRenderer() {}

	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void render() = 0;
	virtual void resize(int w, int h) = 0;
};

#endif
