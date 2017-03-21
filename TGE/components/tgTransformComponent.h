#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../ecs/tgComponent.h"
#include "../math/tgTransform.h"

class tgTransformComponent : public tgComponent {
public:
	tgTransformComponent() : m_transform(new tgTransform()) {}
	tgTransformComponent(tgTransform *transform) : m_transform(transform) {}

	tgTransform *getTransform() const { return m_transform; }
	void setTransform(tgTransform *transform) { m_transform = transform; }

private:
	tgTransform *m_transform;
};

#endif
