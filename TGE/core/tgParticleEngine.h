#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include "../graphics/tgTexture.h"
#include "../graphics/tgRenderer.h"

#include "tgParticleConfiguration.h"
#include "tgEmitter.h"

#include <cmath>

#include <vector>
#include <functional>

using tgParticleTransformCallback = std::function<tgVector2(tgVector2 const&)>;
typedef struct tgParticle {
	tgVector2 position;
	tgVector2 velocity;
	tgVector4 color, deltaColor;

	tgTexture *texture;

	float life = 0;
	float scale, deltaScale;
	float rotation, angularSpeed;
	int z = 0;

	tgParticleTransformCallback transformFunction;

	tgParticleConfiguration config;

	uint emitCounter = 0;

	~tgParticle() = default;
} tgParticle;

class tgParticleEngine {
public:
	tgParticleEngine(uint maxParticles=5000);
	~tgParticleEngine();

	void update(float dt);
	void render(tgRenderer *ren);

	void emit(
		tgTexture *texture,
		tgVector2 const& position,
		tgParticleConfiguration const& config,
		tgParticleTransformCallback cb = nullptr,
		int z = 0
	);

	tgEmitter* createEmitter(tgTexture* tex);

	void restart();

private:
	std::vector<tgParticle*> m_particlePool;
	std::vector<tgEmitter*> m_emitters;

	uint m_particleCount, m_particleIndex, m_maxParticles;

	tgParticle* addParticle(
		tgVector2 const& position,
		tgParticleConfiguration const& config,
		tgParticleTransformCallback cb = nullptr
	);
	void initParticle(tgParticle *p);
	void updateParticle(tgParticle *p, int i, float dt);
	bool isFull() const;

	void cleanup();
};

#endif
