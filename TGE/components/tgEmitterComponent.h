#ifndef EMITTER_COMPONENT_H
#define EMITTER_COMPONENT_H

#include "../ecs/tgComponent.h"
#include "../graphics/tgTexture.h"
#include "../graphics/tgSpriteBatch.h"
#include "../math/tgMath.h"

#include <cmath>
#include <vector>
#include <functional>

#ifndef M_PI
#define M_PI 3.141591654f
#endif

typedef struct tgEmitterConfiguration {
	std::size_t emissionRate = 25;
	float duration = 0.0f;
	bool loop = true;

	tgVector2 positionVar = tgVector2();

	float speed = 0.0f;
	float speedVar = 0.0f;

	float angle = 0;
	float angleVar = 0.0f;

	float life = 1.0f;
	float lifeVar = 0.0f;

	float radius = 0.0f;
	float radiusVar = 0.0f;

	float startScale = 1.0f;
	float startScaleVar = 0.0f;
	float endScale = 0.1f;
	float endScaleVar = 0.0f;

	tgVector4 startColor = tgVector4(1.0f),
		startColorVar = tgVector4(0.0f),
		endColor = tgVector4(1.0f, 1.0f, 1.0f, 0.0f),
		endColorVar = tgVector4(0.0f);

	tgVector2 gravity = tgVector2();

	float radialAccel = 0.0f,
		radialAccelVar = 0.0f,
		tangentialAccel = 0.0f,
		tangentialAccelVar = 0.0f;
} tgEmitterConfiguration;

class tgEmitterComponent : public tgComponent {
public:
	tgEmitterComponent();
	tgEmitterComponent(tgTexture *texture, std::size_t const& maxParticles=100);
	virtual ~tgEmitterComponent() override;

	void update(float dt) override;
	void render(tgRenderer *renderer) override;

	void restart();

	tgEmitterConfiguration getConfiguration() const { return m_config; }
	void setConfiguration(tgEmitterConfiguration val) { m_config = val; }

	tgTexture* getTexture() const { return m_texture; }
	void setTexture(tgTexture * val) { m_texture = val; }

	bool isAdditive() const { return m_additive; }
	void setAdditive(bool val) { m_additive = val; }

	void setTransformFunction(std::function<tgVector2(tgVector2 const &)> val) { m_transformFunction = val; }
private:
	typedef struct tgParticle {
		tgVector2 position;
		tgVector2 velocity, radial, tangential, forces;
		tgVector4 color, deltaColor;
		float life;
		float radialAccel, tangentAccel;

		float scale, dScale;
		float radius;
		float avgSpeed = 0;

		tgParticle();
	} tgParticle;

	bool isFull() const;
	tgParticle* addParticle();
	void initParticle(tgParticle *p);
	void updateParticle(tgParticle *p, float delta, int i);

	tgTexture *m_texture;

	bool m_additive;

	std::vector<tgParticle*> m_particlePool;

	std::function<tgVector2(tgVector2 const&)> m_transformFunction;
	std::size_t m_maxParticles, m_particleCount, m_particleIndex;

	float m_emitCounter, m_elapsed;
	tgVector3 m_position;

	tgEmitterConfiguration m_config;
};

#endif
