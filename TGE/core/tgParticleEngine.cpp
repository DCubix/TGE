#include "tgParticleEngine.h"

#include "../core/tgUtil.h"
#include "../core/tgLog.h"

#include <cstdlib>

static float randF(float LO, float HI) {
	return LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
}

tgParticleEngine::tgParticleEngine(uint maxParticles) {
	m_particleCount = 0;
	m_particleIndex = 0;
	m_maxParticles = maxParticles;
	restart();
}

tgParticleEngine::~tgParticleEngine() {
	for (std::size_t i = 0; i < m_particlePool.size(); i++) {
		tgDelete(m_particlePool[i]);
	}
	for (tgEmitter *e : m_emitters) {
		tgDelete(e);
	}
	m_emitters.clear();
	m_particlePool.clear();
}

void tgParticleEngine::update(float dt) {
	cleanup();

	for (tgEmitter *e : m_emitters) {
		if (!e->m_loop) {
			e->m_elapsed += dt;
		} else {
			e->m_elapsed = 0;
		}

		if (!e->m_enabled) {
			continue;
		}
		if (!e->m_loop) {
			if (e->m_elapsed >= e->m_duration) {
				continue;
			}
		}

		if (e->m_emissionRate > 0) {
			float rate = 1.0f / float(e->m_emissionRate);
			e->m_emitCounter += dt;

			while (!isFull() && e->m_emitCounter > rate) {
				emit(e->m_texture, e->m_position, e->m_config, e->m_cb, e->m_renderOrder);
				e->m_emitCounter -= rate;
			}
		}
	}

	m_particleIndex = 0;

	while (m_particleIndex < m_particleCount) {
		updateParticle(m_particlePool[m_particleIndex], m_particleIndex, dt);
	}
}

void tgParticleEngine::render(tgRenderer *ren) {
	ren->save();
	for (uint i = 0; i < m_particleCount; i++) {
		tgParticle *p = m_particlePool[i];

		ren->setBlendMode(
			p->config.additive ?
			tgBlendMode::tgBLEND_ADD : tgBlendMode::tgBLEND_NORMAL
		);
		ren->setColor(p->color);
		ren->setOrigin(tgVector2(0.5f));
		ren->setPosition(tgVector3(p->position, p->z));
		ren->setRotation(p->rotation);
		ren->setScale(tgVector2(p->scale));
		ren->setUV(tgVector4(0, 0, 1, 1));

		ren->draw(p->texture);
	}
	ren->restore();
}

void tgParticleEngine::emit(tgTexture* texture, tgVector2 const& position, tgParticleConfiguration const& config, tgParticleTransformCallback cb, int z) {
	if (texture) {
		tgParticle *p = addParticle(position, config, cb);
		p->z = z;
		p->texture = texture;
	}
}

tgEmitter* tgParticleEngine::createEmitter(tgTexture* tex) {
	tgEmitter *emitter = new tgEmitter(tex);
	m_emitters.push_back(emitter);
	return m_emitters.back();
}

void tgParticleEngine::restart() {
	if (m_particlePool.size() > 0) {
		for (std::size_t i = 0; i < m_particlePool.size(); i++) {
			tgDelete(m_particlePool[i]);
		}
		m_particlePool.clear();
	}
	for (std::size_t i = 0; i < m_maxParticles; i++) {
		m_particlePool.push_back(new tgParticle());
	}
	for (tgEmitter *e : m_emitters) {
		tgDelete(e);
	}
	m_emitters.clear();
	m_particleCount = 0;
	m_particleIndex = 0;
}

void tgParticleEngine::updateParticle(tgParticle *p, int i, float dt) {
	if (p->life > 0) {
		p->velocity += p->config.gravity * dt;
		p->position += p->velocity * dt;

		p->scale += p->deltaScale * dt;
		p->color += p->deltaColor * dt;

		p->rotation += p->angularSpeed * dt;
		if (p->rotation >= M_PI * 2.0f) {
			p->rotation = 0;
		}

		p->life -= dt;

		++m_particleIndex;
	} else {
		tgParticle *tmp = m_particlePool[i];
		m_particlePool[i] = m_particlePool[m_particleCount - 1];
		m_particlePool[m_particleCount - 1] = tmp;
		--m_particleCount;
	}
}

void tgParticleEngine::initParticle(tgParticle *p) {
	tgVector2 posv = tgVector2(
		p->config.positionVar.x() * randF(-1.0f, 1.0f),
		p->config.positionVar.y() * randF(-1.0f, 1.0f)
	);

	if (p->transformFunction) {
		posv = p->transformFunction(tgVector2(randF(0.0f, 1.0f), randF(-1.0f, 1.0f)));
	}
	p->position = posv + p->position.xy();

	p->angularSpeed = p->config.angularSpeed + p->config.angularSpeedVar * randF(-1.0f, 1.0f);

	float angle = p->config.angle + p->config.angleVar * randF(-1.0f, 1.0f);
	float speed = p->config.speed + p->config.speedVar * randF(-1.0f, 1.0f);

	p->velocity.x() = speed * std::cos(angle);
	p->velocity.y() = speed * std::sin(angle);

	float life = p->config.life + p->config.lifeVar * randF(-1.0f, 1.0f);
	p->life = std::max(0.001f, life);

	float startScale = p->config.startScale + p->config.startScaleVar * randF(0.0f, 1.0f);
	float endScale = p->config.endScale + p->config.endScaleVar * randF(0.0f, 1.0f);
	p->scale = startScale;
	p->deltaScale = (endScale - startScale) / p->life;

	tgVector4 startColor = (p->config.startColor + p->config.startColorVar * randF(0.0f, 1.0f)).clamp(0.0f, 1.0f);
	tgVector4 endColor = (p->config.endColor + p->config.endColorVar * randF(0.0f, 1.0f)).clamp(0.0f, 1.0f);

	p->color = startColor;
	p->deltaColor = (endColor - startColor) / p->life;
}

bool tgParticleEngine::isFull() const {
	return m_particleCount == m_maxParticles;
}

void tgParticleEngine::cleanup() {
	std::vector<uint> rem;
	uint i = 0;
	for (tgEmitter *e : m_emitters) {
		if (!e->m_loop && e->m_elapsed >= e->m_duration) {
			rem.push_back(i);
		}
		i++;
	}

	for (uint i : rem) {
		tgEmitter *e = m_emitters[i];
		m_emitters[i] = m_emitters.back();
		m_emitters.pop_back();
		tgDelete(e);
	}
}

tgParticle* tgParticleEngine::addParticle(tgVector2 const& position,
										  tgParticleConfiguration const& config,
										  tgParticleTransformCallback cb)
{
	if (isFull()) {
		return nullptr;
	}

	tgParticle *p = m_particlePool[m_particleCount];
	p->position = position;
	p->config = config;
	p->transformFunction = cb;
	initParticle(p);
	++m_particleCount;

	return p;
}