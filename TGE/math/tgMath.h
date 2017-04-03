#ifndef MATH_H
#define MATH_H

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include "tgMatrixBase.h"
#include "tgVectorBase.h"

typedef tgVectorBase<2> tgVector2;
typedef tgVectorBase<3> tgVector3;
typedef tgVectorBase<4> tgVector4;

typedef tgMatrixBase<3> tgMatrix3;
typedef tgMatrixBase<4> tgMatrix4;

#endif
