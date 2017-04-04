#ifndef EMITTER_H
#define EMITTER_H

#include "tgTypes.h"
#include "tgParticleConfiguration.h"
#include "../graphics/tgTexture.h"
#include "../math/tgMath.h"

#include <functional>

using tgParticleTransformCallback = std::function<tgVector2(tgVector2 const&)>;
typedef struct tgEmitter {
	friend class tgParticleEngine;

	tgEmitter(tgTexture *texture)
		: m_texture(texture),
		m_loop(true), m_enabled(true),
		m_elapsed(0), m_duration(0), m_emitCounter(0),
		m_emissionRate(25)
	{}
	tgEmitter(tgTexture *texture, float duration)
		: m_texture(texture),
		m_loop(false), m_enabled(true),
		m_elapsed(0), m_duration(duration), m_emitCounter(0),
		m_emissionRate(25)
	{}

	tgTexture* getTexture() { return m_texture; }
	void setTexture(tgTexture *tex) { m_texture = tex; }

	bool isLoop() const { return m_loop; }
	void setLoop(bool v) { m_loop = v; }

	bool isEnabled() const { return m_enabled; }
	void setEnabled(bool e) { m_enabled = e; }

	float getDuration() const { return m_duration; }
	void setDuration(float v) { m_duration = v; }

	uint getEmissionRate() const { return m_emissionRate; }
	void setEmissionRate(uint v) { m_emissionRate = v; }

	tgParticleConfiguration getConfig() const { return  m_config; }
	void setConfig(tgParticleConfiguration const& v) { m_config = v; }

	tgVector2 getPosition() const { return m_position; }
	void setPosition(tgVector2 const& v) { m_position = v; }

	tgParticleTransformCallback getTransformCallback() const { m_cb; }
	void setTransformCallback(tgParticleTransformCallback const& cb) { m_cb = cb; }

protected:
	tgTexture *m_texture;
	bool m_loop, m_enabled;
	float m_elapsed, m_duration, m_emitCounter;
	uint m_emissionRate;
	tgParticleConfiguration m_config;
	tgVector2 m_position;
	tgParticleTransformCallback m_cb;
} tgEmitter;

#endif
