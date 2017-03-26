#include "tgEmitterComponent.h"

#include "../ecs/tgComponentManager.h"
#include "../components/tgTransformComponent.h"
#include "../core/tgUtil.h"
#include "../core/tgLog.h"

#include <cstdlib>

static float randF(float LO, float HI) {
	return LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
}

tgEmitterComponent::tgParticle::tgParticle() {
	this->life = 0;
}

tgEmitterComponent::tgEmitterComponent(tgSpriteBatch *sb, tgTexture *texture, std::size_t const& maxParticles)
	: m_spriteBatch(sb),
	m_texture(texture),
	m_maxParticles(maxParticles),
	m_particleCount(0)
{
	m_particleCount = 0;
	m_particleIndex = 0;
	m_position = tgVector3();
	
	restart();
}

tgEmitterComponent::tgEmitterComponent(tgSpriteBatch *sb)
	: m_spriteBatch(sb),
	m_texture(nullptr),
	m_maxParticles(100),
	m_particleCount(0)
{
	m_particleCount = 0;
	m_particleIndex = 0;
	m_position = tgVector3();

	restart();
}

tgEmitterComponent::~tgEmitterComponent() {
	for (tgParticle *p : m_particlePool) {
		tgDelete(p);
	}
}

void tgEmitterComponent::update(float dt) {
	if (!m_config.loop) {
		m_elapsed += dt;
	} else {
		m_elapsed = 0;
	}

	if (!m_enabled) {
		return;
	}
	if (!m_config.loop) {
		if (m_elapsed >= m_config.duration) {
			return;
		}
	}

	if (m_config.emissionRate > 0) {
		float rate = 1.0f / float(m_config.emissionRate);
		m_emitCounter += dt;

		while (!isFull() && m_emitCounter > rate) {
			addParticle();
			m_emitCounter -= rate;
		}
	}

	m_particleIndex = 0;

	while (m_particleIndex < m_particleCount) {
		tgParticle *p = m_particlePool[m_particleIndex];
		updateParticle(p, dt, m_particleIndex);
	}
}

void tgEmitterComponent::render() {
	tgTransformComponent *transform = getManager()->getComponent<tgTransformComponent>(getOwner());

	tgVector2 scl(1.0f);
	if (transform) {
		m_position = transform->getTransform()->getWorldPosition();
		scl = transform->getTransform()->getWorldScaling().xy();
	}

	m_spriteBatch->save();
	for (tgParticle *p : m_particlePool) {
		if (p->life > 0.0f) {
			m_spriteBatch->setPosition(tgVector3(p->position, m_position.z() + getRenderingOrder()));
			m_spriteBatch->setScale(scl * p->scale);
			m_spriteBatch->setOrigin(tgVector2(0.5f));
			m_spriteBatch->setColor(p->color);
			if (m_additive) {
				m_spriteBatch->setBlendMode(tgSpriteBatch::tgBLEND_ADD);
			}

			m_spriteBatch->draw(getTexture());
		}
	}
	m_spriteBatch->restore();
}

void tgEmitterComponent::restart() {
	for (std::size_t i = 0; i < m_maxParticles; i++) {
		m_particlePool.push_back(new tgParticle());
	}

	m_particleCount = 0;
	m_particleIndex = 0;
	m_emitCounter = 0;
}

bool tgEmitterComponent::isFull() const {
	return m_particleCount == m_maxParticles;
}

tgEmitterComponent::tgParticle* tgEmitterComponent::addParticle() {
	if (isFull()) {
		return nullptr;
	}

	tgParticle *p = m_particlePool[m_particleCount];
	initParticle(p);
	++m_particleCount;
	return p;
}

void tgEmitterComponent::initParticle(tgParticle *p) {
	tgVector2 posv = tgVector2(
		m_config.positionVar.x() * randF(-1.0f, 1.0f),
		m_config.positionVar.y() * randF(-1.0f, 1.0f)
	);

	if (m_transformFunction) {
		posv = m_transformFunction(posv);
	}
	p->position = posv + m_position.xy();

	float angle = m_config.angle + m_config.angleVar * randF(-1.0f, 1.0f);
	float speed = m_config.speed + m_config.speedVar * randF(-1.0f, 1.0f);

	p->velocity.x() = speed * std::cos(angle);
	p->velocity.y() = speed * std::sin(angle);

	p->radialAccel = m_config.radialAccel + m_config.radialAccelVar * randF(-1.0f, 1.0f);
	p->tangentAccel = m_config.tangentialAccel + m_config.tangentialAccelVar * randF(-1.0f, 1.0f);

	float life = m_config.life + m_config.lifeVar * randF(-1.0f, 1.0f);
	p->life = std::max(0.001f, life);

	float startScale = m_config.startScale + m_config.startScaleVar * randF(0.0f, 1.0f);
	float endScale = m_config.endScale + m_config.endScaleVar * randF(0.0f, 1.0f);
	p->scale = startScale;
	p->dScale = (endScale - startScale) / p->life;

	p->radius = m_config.radius + m_config.radiusVar * randF(0.0f, 1.0f);

	tgVector4 startColor = (m_config.startColor + m_config.startColorVar * randF(0.0f, 1.0f)).clamp(0.0f, 1.0f);
	tgVector4 endColor = (m_config.endColor + m_config.endColorVar * randF(0.0f, 1.0f)).clamp(0.0f, 1.0f);

	p->color = startColor;
	p->deltaColor = (endColor - startColor) / p->life;
}

void tgEmitterComponent::updateParticle(tgParticle *p, float delta, int i) {
	if (p->life > 0) {
		p->forces = tgVector2();
		p->radial = tgVector2();

		if ((p->position != m_position.xy()) && (p->radialAccel || p->tangentAccel)) {
			p->radial = (p->position - m_position.xy()).normalized();
		}
		p->tangential.x() = p->radial.x();
		p->tangential.y() = p->radial.y();

		p->radial *= p->radialAccel;

		float newy = p->tangential.x();
		p->tangential.x() = -p->tangential.y();
		p->tangential.y() = newy;

		p->tangential *= p->tangentAccel;

		p->forces = p->radial + p->tangential + m_config.gravity;
		p->forces *= delta;

		p->velocity += p->forces;

		p->position += p->velocity * delta;

		p->life -= delta;

		p->scale += p->dScale * delta;
		p->color += p->deltaColor * delta;

		p->avgSpeed = p->velocity.length();

		++m_particleIndex;
	} else {
		tgParticle *temp = m_particlePool[i];
		m_particlePool[i] = m_particlePool[m_particleCount - 1];
		m_particlePool[m_particleCount - 1] = temp;

		--m_particleCount;
	}
}
