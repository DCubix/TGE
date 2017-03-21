#include "tgAnimatedSpriteNode.h"

tgAnimatedSpriteNode::tgAnimatedSpriteNode(tgTexture *atlas, int nOfRows, int nOfCols)
	: tgSpriteNode(atlas)
{
	setup(nOfRows, nOfCols);
	m_currentAnimation = "";
}

tgAnimatedSpriteNode* tgAnimatedSpriteNode::setup(int nOfRows, int nOfCols) {
	m_clipRects.clear();

	m_rows = nOfRows;
	m_columns = nOfCols;

	int nframes = nOfRows * nOfCols;
	// pre-compute clipping rectangles for each frame
	float w = 1.0f / nOfCols;
	float h = 1.0f / nOfRows;
	for(int i = 0; i < nframes; i++) {
		tgVector4 rect;
		rect.x() = float(i % nOfCols) * w;
		rect.y() = float(i / nOfCols) * h;
		rect.z() = w;
		rect.w() = h;
		m_clipRects.push_back(rect);
	}

	return this;
}

tgAnimatedSpriteNode* tgAnimatedSpriteNode::addAnimation(std::string const& name, std::vector<int> const& frames) {
	tgAnimation *anim = new tgAnimation();
	anim->frames = std::vector<int>(frames);
	anim->loop = false;
	anim->speed = 0.5f;
	anim->time = 0;
	anim->frameIndex = 0;

	if(m_currentAnimation.empty()) {
		m_currentAnimation = name;
	}

	m_animations.insert({ name, anim });

	return this;
}

tgAnimatedSpriteNode* tgAnimatedSpriteNode::play(std::string const& name, float speed, bool loop) {
	auto pos = m_animations.find(name);
	if(pos != m_animations.end()) {
		tgAnimation *anim = pos->second;
		anim->loop = loop;
		anim->speed = speed;
		m_currentAnimation = name;
	}
	return this;
}

void tgAnimatedSpriteNode::onUpdate(float dt) {
	if (m_currentAnimation.empty()) { return; }
	tgAnimation *anim = m_animations[m_currentAnimation];

	int maxFrames = m_rows * m_columns;
	if(!anim->frames.empty()) {
		maxFrames = anim->frames.size();
	}

	anim->time += dt;
	if(anim->time >= anim->speed) {
		anim->frameIndex++;
		if(anim->frameIndex >= maxFrames) {
			if(anim->loop) {
				anim->frameIndex = 0;
			} else {
				anim->frameIndex = maxFrames - 1;
			}
		}
		anim->time = 0;
	}

	int frame = 0;
	if(!anim->frames.empty()) {
		frame = anim->frames[anim->frameIndex];
	} else {
		frame = anim->frameIndex;
	}

	setClipRectangle(m_clipRects[frame]);
}
