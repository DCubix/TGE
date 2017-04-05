#include "tgSpriteSystem.h"

#include "../ecs/tgECS.h"
#include "../components/tgSprite.h"
#include "../components/tgTransform.h"
#include "../graphics/tgSpriteBatch.h"

void tgSpriteSystem::update(float dt) {
	for (tgEntity *ent : getECS()->with<tgSprite>()) {
		auto s = ent->get<tgSprite>();

		if (s->m_currentAnimation.empty()) { return; }
		if (s->m_clipRects.empty()) { return; }
		tgAnimation *anim = s->m_animations[s->m_currentAnimation];

		int maxFrames = s->m_rows * s->m_columns;
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

		s->setClipRectangle(s->m_clipRects[frame]);
	}
}

void tgSpriteSystem::render(tgRenderer *renderer) {
	tgSpriteBatch *sb = dynamic_cast<tgSpriteBatch*>(renderer);
	if (!sb) { return; }

	for (tgEntity *ent : getECS()->with<tgSprite, tgTransform>()) {
		auto s = ent->get<tgSprite>();
		auto transform = ent->get<tgTransform>();

		tgVector3 pos(0.0f);
		tgVector2 scl(1.0f);
		if (transform) {
			pos = transform->getWorldPosition();
			scl = transform->getWorldScaling().xy();
		}

		if (s->m_texture) {
			float rot = transform->getWorldRotation().toEuler().z();
			pos.z() = s->m_renderOrder;
			sb->save();

			sb->setUV(s->m_clipRectangle);
			sb->setPosition(pos);
			sb->setScale(scl);
			sb->setOrigin(s->m_origin);
			sb->setRotation(rot);
			sb->setColor(s->m_color);

			sb->draw(s->m_texture);

			sb->restore();
		}
	}
}
