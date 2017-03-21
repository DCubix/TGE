#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "tgMath.h"
#include "tgQuaternion.h"

class tgTransform {
public:
	tgTransform();

	tgVector3 getLocalPosition() const { return m_position; }
	void setLocalPosition(tgVector3 const& v) { m_position = v; }

	tgVector3 getLocalScaling() const { return m_scale; }
	void setLocalScaling(tgVector3 const& v) { m_scale = v; }

	tgQuaternion getLocalRotation() const { return m_rotation; }
	void getLocalRotation(tgQuaternion const& v) { m_rotation = v; }

	tgMatrix4 getTransformationMatrix();

	tgVector3 left();
	tgVector3 right();
	tgVector3 down();
	tgVector3 up();
	tgVector3 forward();
	tgVector3 back();

private:
	tgVector3 m_position, m_scale;
	tgQuaternion m_rotation;
};

#endif
