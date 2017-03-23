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
		m_spriteBatch->draw(
			m_texture,
			m_clipRectangle,
			tgVector4(pos.x(), pos.y(),
				float(m_texture->getWidth()) * scl.x(),
				float(m_texture->getHeight()) * scl.y()),
			m_origin,
			rot,
			m_color, pos.z()
		);
	}
}
