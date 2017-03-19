#include "tgTransform.h"

tgTransform::tgTransform() {
	m_position = tgVector3();
	m_rotation = tgQuaternion(0, 0, 0, 1);
	m_scale = tgVector3(1);
}

tgMatrix4 tgTransform::getTransformationMatrix() {
	tgMatrix4 T = tgMatrix4::translation(m_position);
	tgMatrix4 R = m_rotation.toMatrix();
	tgMatrix4 S = tgMatrix4::scaling(m_scale);
	return T * R * S;
}

tgVector3 tgTransform::left() {
	return m_rotation.rotateVector3(tgVector3(-1, 0, 0));
}

tgVector3 tgTransform::right() {
	return m_rotation.rotateVector3(tgVector3(1, 0, 0));
}

tgVector3 tgTransform::down() {
	return m_rotation.rotateVector3(tgVector3(0, -1, 0));
}

tgVector3 tgTransform::up() {
	return m_rotation.rotateVector3(tgVector3(0, 1, 0));
}

tgVector3 tgTransform::forward() {
	return m_rotation.rotateVector3(tgVector3(0, 0, 1));
}

tgVector3 tgTransform::back() {
	return m_rotation.rotateVector3(tgVector3(0, 0, -1));
}
