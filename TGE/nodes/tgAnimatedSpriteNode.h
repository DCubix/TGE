#ifndef ANIMATED_SPRITE_NODE_H
#define ANIMATED_SPRITE_NODE_H

#include "tgSpriteNode.h"

#include <vector>
#include <map>
#include <string>

class tgAnimatedSpriteNode : public tgSpriteNode {
public:
	tgAnimatedSpriteNode(tgTexture *atlas, int nOfRows, int nOfCols);

	tgAnimatedSpriteNode* setup(int nOfRows, int nOfCols);
	tgAnimatedSpriteNode* addAnimation(std::string const& name, std::vector<int> const& frames);

	tgAnimatedSpriteNode* play(std::string const& name, float speed, bool loop);

	void onUpdate(float dt) override;

private:
	typedef struct tgAnimation {
		int frameIndex;
		float time;
		float speed;
		bool loop;
		std::vector<int> frames;
	} tgAnimation;

	int m_rows, m_columns;
	std::vector<tgVector4> m_clipRects;
	std::map<std::string, tgAnimation*> m_animations;
	std::string m_currentAnimation;
};

#endif
