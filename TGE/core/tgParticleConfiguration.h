#ifndef PARTICLE_CONFIGURATION_H
#define PARTICLE_CONFIGURATION_H

#include "../math/tgMath.h"

typedef struct tgParticleConfiguration {
	bool additive = false;

	tgVector2 positionVar = tgVector2();

	float speed = 0.0f;
	float speedVar = 0.0f;

	float angle = 0;
	float angleVar = 0.0f;

	float life = 1.0f;
	float lifeVar = 0.0f;

	float startScale = 1.0f;
	float startScaleVar = 0.0f;
	float endScale = 0.1f;
	float endScaleVar = 0.0f;

	tgVector4 startColor = tgVector4(1.0f),
		startColorVar = tgVector4(1.0f),
		endColor = tgVector4(1.0f),
		endColorVar = tgVector4(1.0f);

	tgVector2 gravity = tgVector2();

} tgParticleConfiguration;

#endif
