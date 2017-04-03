#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math/tgMath.h"
#include "../math/tgQuaternion.h"
#include "../ecs/tgComponent.h"

typedef struct tgTransform : public tgComponent {
	tgTransform();
	~tgTransform() = default;

	tgVector3& getLocalPosition() { return m_position; }
	void setLocalPosition(tgVector3 const& v) { m_position = v; }

	tgVector3& getLocalScaling() { return m_scale; }
	void setLocalScaling(tgVector3 const& v) { m_scale = v; }

	tgQuaternion& getLocalRotation() { return m_rotation; }
	void getLocalRotation(tgQuaternion const& v) { m_rotation = v; }

	tgVector3 getWorldPosition();
	tgVector3 getWorldScaling();
	tgQuaternion getWorldRotation();

	tgMatrix4 getTransformationMatrix();

	tgVector3 left();
	tgVector3 right();
	tgVector3 down();
	tgVector3 up();
	tgVector3 forward();
	tgVector3 back();

	void rotate(tgVector3 const& axis, float angle);

	tgTransform *getParent() const { return m_parent; }
	void setParent(tgTransform *parent);

private:
	tgVector3 m_position, m_scale;
	tgQuaternion m_rotation;

	tgTransform *m_parent;
	tgMatrix4 getParentTransformationMatrix();
} tgTransform;

#endif
