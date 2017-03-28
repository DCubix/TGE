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
