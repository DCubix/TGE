#include "tgTransform.h"

tgTransform::tgTransform() {
	m_position = tgVector3(0, 0, 0);
	m_rotation = tgQuaternion(0, 0, 0, 1);
	m_scale = tgVector3(1);
	m_parent = nullptr;
}

tgVector3 tgTransform::getWorldPosition() {
	return getParentTransformationMatrix() * getLocalPosition();
}

tgVector3 tgTransform::getWorldScaling() {
	return getParentTransformationMatrix() * getLocalScaling();
}

tgQuaternion tgTransform::getWorldRotation() {
	tgQuaternion rot(0, 0, 0, 1);
	if (m_parent) {
		rot = m_parent->getWorldRotation();
	}
	return rot * getLocalRotation();
}

tgMatrix4 tgTransform::getTransformationMatrix() {
	tgMatrix4 T = tgMatrix4::translation(m_position);
	tgMatrix4 R = m_rotation.toMatrix();
	tgMatrix4 S = tgMatrix4::scaling(m_scale);
	return getParentTransformationMatrix() * (T * R * S);
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

void tgTransform::rotate(tgVector3 const& axis, float angle) {
	m_rotation = m_rotation * tgQuaternion(axis, angle);
}

void tgTransform::setParent(tgTransform *parent) {
	if (m_parent == nullptr) {
		if (parent != nullptr) {
			m_parent = parent;
			tgVector3 WtoL = getLocalPosition() - parent->getWorldPosition();
			setLocalPosition(WtoL);
		}
	}
	else {
		if (parent == nullptr) {
			tgVector3 LtoW = getLocalPosition() + m_parent->getWorldPosition();
			m_parent = nullptr;
			setLocalPosition(LtoW);
		}
		else {
			m_parent = nullptr;
			setParent(parent);
		}
	}
}

tgMatrix4 tgTransform::getParentTransformationMatrix() {
	tgMatrix4 parent = tgMatrix4::identity();
	if (m_parent != nullptr) {
		parent = m_parent->getTransformationMatrix();
	}
	return parent;
}
