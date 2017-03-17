#ifndef VECTOR_BASE_H
#define VECTOR_BASE_H

#include <cmath>
#include <assert.h>
#include <iomanip>

template <unsigned C>
class tgVectorBase {
	using VBASE = tgVectorBase<C>;
public:
	tgVectorBase() {}
	tgVectorBase(float v) {
		for(int i = 0; i < C; i++) {
			m_data[i] = v;
		}
	}
	tgVectorBase(float x, float y) {
		static_assert (C >= 2, "This vector doesn't have 2 components");
		m_data[0] = x;
		m_data[1] = y;
	}
	tgVectorBase(float x, float y, float z) {
		static_assert (C >= 3, "This vector doesn't have 3 components");
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
	}
	tgVectorBase(float x, float y, float z, float w) {
		static_assert (C >= 4, "This vector doesn't have 4 components");
		m_data[0] = x;
		m_data[1] = y;
		m_data[2] = z;
		m_data[3] = w;
	}
	tgVectorBase(tgVectorBase<2> const& o, float z) {
		static_assert (C >= 3, "This vector doesn't have 3 components");
		m_data[0] = o[0];
		m_data[1] = o[1];
		m_data[2] = z;
	}
	tgVectorBase(tgVectorBase<2> const& o, float z, float w) {
		static_assert (C >= 4, "This vector doesn't have 4 components");
		m_data[0] = o[0];
		m_data[1] = o[1];
		m_data[2] = z;
		m_data[3] = w;
	}
	tgVectorBase(tgVectorBase<3> const& o, float w) {
		static_assert (C >= 4, "This vector doesn't have 4 components");
		m_data[0] = o[0];
		m_data[1] = o[1];
		m_data[2] = o[2];
		m_data[3] = w;
	}

	// Swizzling
	tgVectorBase<2> xy() {
		return tgVectorBase<2>(m_data[0], m_data[1]);
	}
	tgVectorBase<2> xx() {
		return tgVectorBase<2>(m_data[0], m_data[0]);
	}
	tgVectorBase<2> yy() {
		return tgVectorBase<2>(m_data[1], m_data[1]);
	}
	tgVectorBase<2> st() {
		return tgVectorBase<2>(m_data[0], m_data[1]);
	}
	tgVectorBase<3> xyz() {
		return tgVectorBase<3>(m_data[0], m_data[1], m_data[2]);
	}
	tgVectorBase<3> str() {
		return tgVectorBase<3>(m_data[0], m_data[1], m_data[2]);
	}
	tgVectorBase<4> xyzw() {
		return tgVectorBase<4>(m_data[0], m_data[1], m_data[2], m_data[3]);
	}
	tgVectorBase<4> xyww() {
		return tgVectorBase<4>(m_data[0], m_data[1], m_data[3], m_data[3]);
	}

	float dot(VBASE const& b) const {
		float dp = 0.0f;
		for(unsigned i = 0; i < C; i++) {
			dp += m_data[i] * b.m_data[i];
		}
		return dp;
	}

	VBASE cross(VBASE const& o) const {
		static_assert (C == 3, "Cross products are only supported by Vector3");
		VBASE ret;
		ret[0] = (*this)[1] * o[2] + (*this)[2] * o[1];
		ret[1] = (*this)[2] * o[0] + (*this)[0] * o[2];
		ret[2] = (*this)[0] * o[1] + (*this)[1] * o[0];
		return ret;
	}

	float lengthSquared() const {
		return this->dot(*this);
	}

	float length() const {
		return std::sqrt(lengthSquared());
	}

	VBASE normalized() const {
		float len = length();
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] / len;
		}
		return ret;
	}

	float& x() { return m_data[0]; }
	float& y() { return m_data[1]; }
	float& z() { return m_data[2]; }
	float& w() { return m_data[3]; }

	float x() const { return m_data[0]; }
	float y() const { return m_data[1]; }
	float z() const { return m_data[2]; }
	float w() const { return m_data[3]; }

	// Operations
	VBASE operator + (VBASE const& b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] + b.m_data[i];
		}
		return ret;
	}

	VBASE operator - (VBASE const& b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] - b.m_data[i];
		}
		return ret;
	}

	VBASE operator * (VBASE const& b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] * b.m_data[i];
		}
		return ret;
	}

	VBASE operator / (VBASE const& b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] / b.m_data[i];
		}
		return ret;
	}

	VBASE& operator += (VBASE const& b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] += b.m_data[i];
		}
		return *this;
	}

	VBASE& operator -= (VBASE const& b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] -= b.m_data[i];
		}
		return *this;
	}

	VBASE& operator *= (VBASE const& b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] *= b.m_data[i];
		}
		return *this;
	}

	VBASE& operator /= (VBASE const& b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] /= b.m_data[i];
		}
		return *this;
	}

	// Float operations
	VBASE operator + (float b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] + b;
		}
		return ret;
	}

	VBASE operator - (float b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] - b;
		}
		return ret;
	}

	VBASE operator * (float b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] * b;
		}
		return ret;
	}

	VBASE operator / (float b) const {
		VBASE ret;
		for(unsigned i = 0; i < C; i++) {
			ret.m_data[i] = m_data[i] / b;
		}
		return ret;
	}

	VBASE& operator += (float b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] += b;
		}
		return *this;
	}

	VBASE& operator -= (float b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] -= b;
		}
		return *this;
	}

	VBASE& operator *= (float b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] *= b;
		}
		return *this;
	}

	VBASE& operator /= (float b) {
		for(unsigned i = 0; i < C; i++) {
			m_data[i] /= b;
		}
		return *this;
	}

	// Equality
	bool operator == (VBASE const& o) const {
		for(unsigned i = 0; i < C; i++) {
			if(m_data[i] != o.m_data[i]) {
				return false;
			}
		}
		return true;
	}

	bool operator != (VBASE const& o) const {
		return !(*this == o);
	}

	// Negation
	VBASE operator -() const {
		return (*this) * -1.0f;
	}

	float& operator [] (unsigned i) {
		assert(i < C);
		return m_data[i];
	}

	const float& operator [] (unsigned i) const {
		assert(i < C);
		return m_data[i];
	}

	friend std::ostream& operator<< (std::ostream& os, VBASE const& o) {
		os << "Vector<" << C << ">";
		os << "( ";
		for(unsigned i = 0; i < C; i++) {
			os.width(5);
			os << std::fixed << std::setprecision(3) << o[i] << " ";
		}
		os << ")" << std::endl;
		return os;
	}

protected:
	float m_data[C];
};

#endif // VECTOR_BASE_H
