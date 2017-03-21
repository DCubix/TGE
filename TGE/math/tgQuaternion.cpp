#include "tgQuaternion.h"

#include <cmath>

tgQuaternion::tgQuaternion() {
	(*this)[0] = 0;
	(*this)[1] = 0;
	(*this)[2] = 0;
	(*this)[3] = 1;
}

tgQuaternion::tgQuaternion(tgQuaternion const& q) {
	(*this)[0] = q.x();
	(*this)[1] = q.y();
	(*this)[2] = q.z();
	(*this)[3] = q.w();
}

tgQuaternion::tgQuaternion(float x, float y, float z, float w) {
	(*this)[0] = x;
	(*this)[1] = y;
	(*this)[2] = z;
	(*this)[3] = w;
}

tgQuaternion::tgQuaternion(tgVector3 const& axis, float angle) {
	float sinHalfAngle = std::sin(angle / 2.0f);
	float cosHalfAngle = std::cos(angle / 2.0f);

	(*this)[0] = axis.x() * sinHalfAngle;
	(*this)[1] = axis.y() * sinHalfAngle;
	(*this)[2] = axis.z() * sinHalfAngle;
	(*this)[3] = cosHalfAngle;
}

float tgQuaternion::dot(tgQuaternion const & q) const {
	return m_x * q.x() + m_y * q.y() + m_z * q.z() + m_w * q.w();
}

float tgQuaternion::length() const {
	return this->dot((*this));
}

tgQuaternion tgQuaternion::normalized() const {
	float l = length();
	return tgQuaternion(m_x / l, m_y / l, m_z / l, m_w / l);
}

tgQuaternion tgQuaternion::conjugated() {
	return tgQuaternion(-m_x, -m_y, -m_z, m_w);
}

tgQuaternion tgQuaternion::nLerp(tgQuaternion const& dest, float factor, bool shortest) {
	tgQuaternion correctedDest = dest;

	if (shortest && this->dot(dest) < 0) {
		correctedDest = tgQuaternion(-dest.x(), -dest.y(), -dest.z(), -dest.w());
	}

	return ((correctedDest - (*this)) * factor) + this->normalized();
}

tgQuaternion tgQuaternion::sLerp(tgQuaternion const& dest, float factor, bool shortest) {
	const float EPSILON = 1e3f;

	float cos = this->dot(dest);
	tgQuaternion correctedDest = dest;

	if (shortest && cos < 0) {
		cos = -cos;
		correctedDest = tgQuaternion(-dest.x(), -dest.y(), -dest.z(), -dest.w());
	}

	if (std::abs(cos) >= 1 - EPSILON) {
		return nLerp(correctedDest, factor, false);
	}

	float sin = std::sqrt(1.0f - cos * cos);
	float angle = std::atan2(sin, cos);
	float invSin = 1.0f / sin;

	float srcFactor = std::sin((1.0f - factor) * angle) * invSin;
	float destFactor = std::sin((factor) * angle) * invSin;

	return ((*this) * srcFactor) + (correctedDest * destFactor);
}

tgQuaternion tgQuaternion::operator*(float r) const {
	return tgQuaternion(m_x * r, m_y * r, m_z * r, m_w * r);
}

tgQuaternion tgQuaternion::operator*(tgQuaternion const& r) const {
	float w_ = w() * r.w() - x() * r.x() - y() * r.y() - z() * r.z();
	float x_ = x() * r.w() + w() * r.x() + y() * r.z() - z() * r.y();
	float y_ = y() * r.w() + w() * r.y() + z() * r.x() - x() * r.z();
	float z_ = z() * r.w() + w() * r.z() + x() * r.y() - y() * r.x();

	return tgQuaternion(x_, y_, z_, w_);
}

tgQuaternion tgQuaternion::operator*(tgVector3 const& r) const {
	float w_ = -x() * r.x() - y() * r.y() - z() * r.z();
	float x_ =  w() * r.x() + y() * r.z() - z() * r.y();
	float y_ =  w() * r.y() + z() * r.x() - x() * r.z();
	float z_ =  w() * r.z() + x() * r.y() - y() * r.x();

	return tgQuaternion(x_, y_, z_, w_);
}

tgQuaternion tgQuaternion::operator+(tgQuaternion const& r) const {
	return tgQuaternion(x() + r.x(), y() + r.y(), z() + r.z(), w() + r.w());
}

tgQuaternion tgQuaternion::operator-(tgQuaternion const& r) const {
	return tgQuaternion(x() - r.x(), y() - r.y(), z() - r.z(), w() - r.w());
}

tgVector3 tgQuaternion::rotateVector3(tgVector3 const & v) {
	tgQuaternion conj = this->conjugated();
	tgQuaternion w = ((*this) * v) * conj;
	return tgVector3(w.x(), w.y(), w.z());
}

tgMatrix4 tgQuaternion::toMatrix() {
	tgVector3 forward = tgVector3(2.0f * (x() * z() - w() * y()), 2.0f * (y() * z() + w() * x()), 1.0f - 2.0f * (x() * x() + y() * y()));
	tgVector3 up	  = tgVector3(2.0f * (x() * y() + w() * z()), 1.0f - 2.0f * (x() * x() + z() * z()), 2.0f * (y() * z() - w() * x()));
	tgVector3 right	  = tgVector3(1.0f - 2.0f * (y() * y() + z() * z()), 2.0f * (x() * y() - w() * z()), 2.0f * (x() * z() + w() * y()));

	return tgMatrix4::rotation(forward, up, right);
}

tgVector3 tgQuaternion::toEuler() {
	float ey = std::atan2(2.0f * (y() * z() + w() * x()), w() * w() - x() * x() - y() * y() + z() * z());
	float ez = std::asin(-2.0f * (x() * z() - w() * y()));
	float ex = std::atan2(2.0f * (x() * y() + w() * z()), w() * w() + x() * x() - y() * y() - z() * z());
	return tgVector3(ex, ey, ez);
}
