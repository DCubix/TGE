#ifndef POST_EFFECT_H
#define POST_EFFECT_H

#include "tgShaderProgram.h"

class tgPostEffect : public tgShaderProgram {
public:
	tgPostEffect();

	virtual void update() {}

	void setShader(std::string const& src);

	int getRepeatCount() const { return m_repeatCount; }
	void setRepeatCount(int val) { m_repeatCount = val; }
private:
	int m_repeatCount;
	bool m_set;
};

#endif
