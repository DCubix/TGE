#ifndef QUATERNION_H
#define QUATERNION_H

#include "tgMath.h"

class tgQuaternion {
public:
	tgQuaternion();
	tgQuaternion(tgQuaternion const& q);
	tgQuaternion(float x, float y, float z, float w);
	tgQuaternion(tgVector3 const& axis, float angle);

	float dot(tgQuaternion const& q) const;
	float length() const;
	tgQuaternion normalized() const;
	tgQuaternion conjugated();
	tgQuaternion nLerp(tgQuaternion const& dest, float factor, bool shortest);
	tgQuaternion sLerp(tgQuaternion const& dest, float factor, bool shortest);

	tgQuaternion operator *(float r) const;
	tgQuaternion operator *(tgQuaternion const& r) const;
	tgQuaternion operator *(tgVector3 const& r) const;
	tgQuaternion operator +(tgQuaternion const& r) const;
	tgQuaternion operator -(tgQuaternion const& r) const;

	tgVector3 rotateVector3(tgVector3 const& v);

	tgMatrix4 toMatrix();
	tgVector3 toEuler();

	float& operator [](unsigned i) {
		switch(i) {
			case 0: return m_x;
			case 1: return m_y;
			case 2: return m_z;
			case 3: return m_w;
		}
		return m_w;
	}

	const float& operator [](unsigned i) const {
		switch(i) {
			case 0: return m_x;
			case 1: return m_y;
			case 2: return m_z;
			case 3: return m_w;
		}
		return m_w;
	}

	float& x() { return m_x; }
	float& y() { return m_y; }
	float& z() { return m_z; }
	float& w() { return m_w; }

	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }
	float w() const { return m_w; }
private:
	float m_x, m_y, m_z, m_w;
};

#endif