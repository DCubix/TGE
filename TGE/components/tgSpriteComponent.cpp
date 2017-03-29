#include "tgSpriteComponent.h"

#include "../ecs/tgComponentManager.h"
#include "../components/tgTransformComponent.h"

void tgSpriteComponent::render(tgRenderer *renderer) {
	tgSpriteBatch *sb = dynamic_cast<tgSpriteBatch*>(renderer);
	if (!sb) { return; }

	tgTransformComponent *transform = getManager()->getComponent<tgTransformComponent>(getOwner());

	tgVector3 pos(0.0f);
	tgVector2 scl(1.0f);
	if (transform) {
		pos = transform->getTransform()->getWorldPosition();
		scl = transform->getTransform()->getWorldScaling().xy();
	}

	if (m_texture) {
		float rot = transform->getTransform()->getWorldRotation().toEuler().z();
		sb->save();

		sb->setUV(m_clipRectangle);
		sb->setPosition(pos);
		sb->setScale(scl);
		sb->setOrigin(m_origin);
		sb->setRotation(rot);
		sb->setColor(m_color);

		sb->draw(m_texture);

		sb->restore();
	}
}

void tgSpriteComponent::update(float dt) {
	if (m_currentAnimation.empty()) { return; }
	if (m_clipRects.empty()) { return; }
	tgAnimation *anim = m_animations[m_currentAnimation];

	int maxFrames = m_rows * m_columns;
	if (!anim->frames.empty()) {
		maxFrames = anim->frames.size();
	}

	anim->time += dt;
	if (anim->time >= anim->speed) {
		anim->frameIndex++;
		if (anim->frameIndex >= maxFrames) {
			if (anim->loop) {
				anim->frameIndex = 0;
			} else {
				anim->frameIndex = maxFrames - 1;
			}
		}
		anim->time = 0;
	}

	int frame = 0;
	if (!anim->frames.empty()) {
		frame = anim->frames[anim->frameIndex];
	} else {
		frame = anim->frameIndex;
	}

	setClipRectangle(m_clipRects[frame]);
}

tgSpriteComponent* tgSpriteComponent::setup(int nOfRows, int nOfCols) {
	m_clipRects.clear();

	m_rows = nOfRows;
	m_columns = nOfCols;

	int nframes = nOfRows * nOfCols;
	// pre-compute clipping rectangles for each frame
	float w = 1.0f / nOfCols;
	float h = 1.0f / nOfRows;
	for (int i = 0; i < nframes; i++) {
		tgVector4 rect;
		rect.x() = float(i % nOfCols) * w;
		rect.y() = float(i / nOfCols) * h;
		rect.z() = w;
		rect.w() = h;
		m_clipRects.push_back(rect);
	}

	return this;
}

tgSpriteComponent* tgSpriteComponent::addAnimation(std::string const& name, std::vector<int> const& frames) {
	tgAnimation *anim = new tgAnimation();
	anim->frames = std::vector<int>(frames);
	anim->loop = false;
	anim->speed = 0.5f;
	anim->time = 0;
	anim->frameIndex = 0;

	if (m_currentAnimation.empty()) {
		m_currentAnimation = name;
	}

	m_animations[name] = anim;

	return this;
}

tgSpriteComponent* tgSpriteComponent::play(std::string const& name, float speed, bool loop) {
	auto pos = m_animations.find(name);
	if (pos != m_animations.end()) {
		tgAnimation *anim = pos->second;
		anim->loop = loop;
		anim->speed = speed;
		m_currentAnimation = name;
	}
	return this;
}
