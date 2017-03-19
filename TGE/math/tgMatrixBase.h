#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include "tgVectorBase.h"
#include <iomanip>
#include <algorithm>

template <unsigned C>
class tgMatrixBase {
	using MBASE = tgMatrixBase<C>;
public:
	tgMatrixBase() {
		for(unsigned x = 0; x < C; x++) {
			for(unsigned y = 0; y < C; y++) {
				(*this)[x][y] = 0.0f;
			}
		}
	}

	MBASE operator * (MBASE const& o) {
		MBASE ret;
		for(unsigned x = 0; x < C; x++) {
			for(unsigned y = 0; y < C; y++) {
				for(unsigned k = 0; k < C; k++) {
					ret[x][y] += (*this) [x][k] * o[k][y];
				}
			}
		}
		return ret;
	}

	tgVectorBase<4> operator * (tgVectorBase<4> const& o) {
		tgVectorBase<4> ret;
		ret[0] = (o[0] * (*this) [0][0]) + (o[1] * (*this) [1][0]) + (o[2] * (*this) [2][0]) + (o[3] * (*this) [3][0]);
		ret[1] = (o[0] * (*this) [0][1]) + (o[1] * (*this) [1][1]) + (o[2] * (*this) [2][1]) + (o[3] * (*this) [3][1]);
		ret[2] = (o[0] * (*this) [0][2]) + (o[1] * (*this) [1][2]) + (o[2] * (*this) [2][2]) + (o[3] * (*this) [3][2]);
		ret[3] = (o[0] * (*this) [0][3]) + (o[1] * (*this) [1][3]) + (o[2] * (*this) [2][3]) + (o[3] * (*this) [3][3]);
		return ret;
	}

	tgVectorBase<3> operator * (tgVectorBase<3> const& o) {
		tgVectorBase<4> ret = tgVectorBase<4>(o, 1.0f);
		return ((*this) * ret).xyz();
	}

	MBASE operator * (float o) {
		MBASE ret;
		for(unsigned x = 0; x < C; x++) {
			for(unsigned y = 0; y < C; y++) {
				ret[x][y] = (*this) [x][y] * o;
			}
		}
		return ret;
	}

	float determinant() {
		if(C == 1) {
			return (*this) [0][0];
		} else if(C == 2) {
			return (*this) [0][0] * (*this) [1][1] - (*this) [1][0] * (*this) [0][1];
		} else {
			MBASE a = (*this);
			float det = 0.0f;
			for(unsigned i = 0; i < C; i++) {
				det = det + (a[0][i] * (a[1][(i + 1) % C] * a[2][(i + 2) % C]) -
							(a[1][(i + 2) % C] * a[2][(i + 1) % C]));
			}
			return det;
		}
	}

	MBASE inverted() {
		float det = determinant();
		if(det == 0) {
			return MBASE(); // Doesn't have determinant, so it's not invertable.
		}
		float inv_det = 1.0f / det;
		MBASE a = (*this);
		MBASE ret;
		for(unsigned i = 0; i < C; i++) {
			for(unsigned j = 0; j < C; j++) {
				ret(i, j) = inv_det *
					(a[(i + 1) % C][(j + 1) % C] * a[(i + 2) % C][(j + 2) % C)] -
					(a[(i + 1) % C][(j + 2) % C] * a[(i + 2) % C][(j + 1) % C)];
			}
		}
		return ret;
	}

	MBASE trasposed() {
		MBASE ret;
		for(unsigned i = 0; i < C; i++) {
			for(unsigned j = 0; j < C; j++) {
				ret[i][j] = (*this) [j][i];
			}
		}
		return ret;
	}

	float* operator [](unsigned i) {
		return m[i];
	}

	const float* operator [](unsigned i) const {
		return m[i];
	}
	
	static MBASE identity() {
		MBASE ret;
		for(unsigned i = 0; i < C; i++) {
			for(unsigned j = 0; j < C; j++) {
				if(i == j) {
					ret[i][j] = 1.0f;
				} else {
					ret[i][j] = 0.0f;
				}
			}
		}
		return ret;
	}

	static tgMatrixBase<4> translation(tgVectorBase<3> const& t) {
		tgMatrixBase<4> ret = tgMatrixBase<4>::identity();
		ret[0][3] = t.x();
		ret[1][3] = t.y();
		ret[2][3] = t.z();
		return ret;
	}

	static MBASE rotation(float angle, tgVectorBase<3> const& ax) {
		static_assert (C == 4 || C == 3, "This is not a Matrix4x4 or Matrix3x3");
		MBASE ret = MBASE::identity();
		float c = std::cos(angle);
		float s = std::sin(angle);
		float t = 1.0f - c;

		tgVectorBase<3> naxis = ax.normalized();
		float x = naxis.x();
		float y = naxis.y();
		float z = naxis.z();

		ret[0][0] = 1 + t * (x * x - 1);
		ret[0][1] = z * s + t * x * y;
		ret[0][2] = -y * s + t * x * z;

		ret[1][0] = -z * s + t * x * y;
		ret[1][1] = 1 + t * (y * y - 1);
		ret[1][2] = x * s + t * y * z;

		ret[2][0] = y * s + t * x * z;
		ret[2][1] = -x * s + t * y * z;
		ret[2][2] = 1 + t * (z * z - 1);

		return ret;
	}

	static MBASE rotation(tgVectorBase<3> const& f, tgVectorBase<3> const& u, tgVectorBase<3> const& r) {
		static_assert (C == 4 || C == 3, "This is not a Matrix4x4 or Matrix3x3");
		MBASE ret = MBASE::identity();
		ret[0][0] = r.x();
		ret[0][1] = r.y();
		ret[0][2] = r.z();
		ret[1][0] = u.x();
		ret[1][1] = u.y();
		ret[1][2] = u.z();
		ret[2][0] = f.x();
		ret[2][1] = f.y();
		ret[2][2] = f.z();
		return ret;
	}

	static MBASE scaling(tgVectorBase<3> const& s) {
		static_assert (C == 4 || C == 3, "This is not a Matrix4x4 or Matrix3x3");
		MBASE ret = MBASE::identity();
		ret[0][0] = s.x();
		ret[1][1] = s.y();
		ret[2][2] = s.z();
		return ret;
	}

	static tgMatrixBase<4> perspective(float fov, float aspect, float znear, float zfar) {
		float cotFov = 1.0f / std::tan(fov / 2.0f);
		float cotFovAspect = cotFov / aspect;

		tgMatrixBase<4> ret = tgMatrixBase<4>::identity();
		ret[0][0] = cotFovAspect;
		ret[1][1] = cotFov;
		ret[2][2] = zfar / (zfar - znear);
		ret[3][2] = -1.0f;
		ret[2][3] = (znear * zfar) / (zfar - znear);
		ret[3][3] = 0.0f;
		return ret;
	}

	static tgMatrixBase<4> ortho(float l, float r, float b, float t, float znear, float zfar) {
		float width = r - l;
		float height = t - b;
		float depth = zfar - znear;

		tgMatrixBase<4> ret = tgMatrixBase<4>::identity();
		ret[0][0] = 2 / width;
		ret[0][1] = 0;
		ret[0][2] = 0;
		ret[0][3] = -(r + l) / width;
		ret[1][0] = 0;
		ret[1][1] = 2 / height;
		ret[1][2] = 0;
		ret[1][3] = -(t + b) / height;
		ret[2][0] = 0;
		ret[2][1] = 0;
		ret[2][2] = -2 / depth;
		ret[2][3] = -(zfar + znear) / depth;
		ret[3][0] = 0;
		ret[3][1] = 0;
		ret[3][2] = 0;
		ret[3][3] = 1;
		return ret;
	}

	static tgMatrixBase<4> lookAt(tgVectorBase<3> const& eye, tgVectorBase<3> const& at, tgVectorBase<3> const& up) {
		tgVectorBase<3> x, y, z;
		z = (eye - at).normalized();
		x = up.cross(z).normalized();
		y = z.cross(x).normalized();

		tgMatrixBase<4> ret = tgMatrixBase<4>::identity();
		ret[0][0] = x.x();
		ret[1][0] = x.y();
		ret[2][0] = x.z();
		ret[3][0] = -x.dot(eye);

		ret[0][1] = y.x();
		ret[1][1] = y.y();
		ret[2][1] = y.z();
		ret[3][1] = -y.dot(eye);

		ret[0][2] = z.x();
		ret[1][2] = z.y();
		ret[2][2] = z.z();
		ret[3][2] = -z.dot(eye);

		ret[0][3] = 0.0f;
		ret[1][3] = 0.0f;
		ret[2][3] = 0.0f;
		ret[3][3] = 1.0f;
		return ret;
	}


	friend std::ostream& operator<< (std::ostream& os, MBASE const& o) {
		for(unsigned i = 0; i < C; i++) {
			os << "[ ";
			for(unsigned j = 0; j < C; j++) {
				os.width(7);
				os << std::fixed << std::setprecision(4) << o[i][j] << " ";
			}
			os << "]" << std::endl;
		}
		return os;
	}

private:
	float m[C][C];
};

#endif // MATRIX_BASE_H
