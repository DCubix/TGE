#include "tgSpriteComponent.h"

#include "../ecs/tgComponentManager.h"
#include "../components/tgTransformComponent.h"

void tgSpriteComponent::render() {
	tgTransformComponent *transform = getManager()->getComponent<tgTransformComponent>(getOwner());

	tgVector3 pos(0.0f);
	tgVector2 scl(1.0f);
	if (transform) {
		pos = transform->getTransform()->getWorldPosition();
		scl = transform->getTransform()->getWorldScaling().xy();
	}

	if (m_texture) {
		float rot = transform->getTransform()->getWorldRotation().toEuler().z();
		m_spriteBatch->save();

		m_spriteBatch->setUV(m_clipRectangle);
		m_spriteBatch->setPosition(pos);
		m_spriteBatch->setScale(scl);
		m_spriteBatch->setOrigin(m_origin);
		m_spriteBatch->setRotation(rot);
		m_spriteBatch->setColor(m_color);

		m_spriteBatch->draw(m_texture);

		m_spriteBatch->restore();
	}
}
