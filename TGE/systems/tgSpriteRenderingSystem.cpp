#include "tgSpriteRenderingSystem.h"

#include "../core/tgUtil.h"
#include "../ecs/tgEntitySystemManager.h"

#include "../components/tgSpriteComponent.h"
#include "../components/tgTransformComponent.h"

tgSpriteRenderingSystem::tgSpriteRenderingSystem(tgSpriteBatch *spriteBatch) {
	m_spriteBatch = spriteBatch;
}

tgSpriteRenderingSystem::~tgSpriteRenderingSystem() {
}

void tgSpriteRenderingSystem::render(tgEntitySystemManager *mgr) {
	m_spriteBatch->begin();

	for (tgEntity *entity : mgr->search<tgSpriteComponent, tgTransformComponent>()) {
		tgComponentHandler<tgSpriteComponent> sprite = entity->get<tgSpriteComponent>();
		tgComponentHandler<tgTransformComponent> transform = entity->get<tgTransformComponent>();

		tgTexture *tex = sprite->getTexture();
		if (tex) {
			tgVector2 pos = transform->getTransform()->getWorldPosition().xy();
			tgVector2 scl = transform->getTransform()->getWorldScaling().xy();
			float rot = transform->getTransform()->getWorldRotation().toEuler().z();
			m_spriteBatch->draw(
				sprite->getTexture(),
				sprite->getClipRectangle(),
				tgVector4(pos.x(), pos.y(), float(tex->getWidth()) * scl.x(), float(tex->getHeight()) * scl.y()),
				sprite->getOrigin(),
				rot,
				sprite->getColor()
			);
		}
	}

	m_spriteBatch->end();
}
